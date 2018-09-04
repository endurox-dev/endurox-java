/**
 * @brief Java UBF Backing routines
 *
 * @file TypedUbf_Bprint.c
 */
/* -----------------------------------------------------------------------------
 * Enduro/X Middleware Platform for Distributed Transaction Processing
 * Copyright (C) 2009-2016, ATR Baltic, Ltd. All Rights Reserved.
 * Copyright (C) 2017-2018, Mavimax, Ltd. All Rights Reserved.
 * This software is released under one of the following licenses:
 * GPL or Mavimax's license for commercial use.
 * -----------------------------------------------------------------------------
 * GPL license:
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * -----------------------------------------------------------------------------
 * A commercial use license is available from Mavimax, Ltd
 * contact@mavimax.com
 * -----------------------------------------------------------------------------
 */

/*---------------------------Includes-----------------------------------*/
#include <jni.h>
#include <errno.h>
#include <stdlib.h>
#include "org_endurox_AtmiCtx.h"
#include "org_endurox_TypedBuffer.h"
#include "org_endurox_TypedUbf.h"
#include <atmi.h>
#include <oatmi.h>
#include <ndebug.h>
#include <ondebug.h>
#include <oatmisrv_integra.h>
#include "libsrc.h"
#include <sys_unix.h>
#include "nerror.h"
#include <ndrstandard.h>
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/

/**
 * Print the UBF buffer to STDOUT
 * @param env java env
 * @param data TypedUbf object
 */
expublic void JNICALL Java_org_endurox_TypedUbf_Bprint(JNIEnv *env, jobject data)
{
    char *cdata;
    long clen;
    
    /* get the context, switch */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
       return; 
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, &clen))
    {
        UBF_LOG(log_error, "Failed to get buffer data");
        goto out;
    }
    
    if (EXSUCCEED!=Bprint((UBFH *)cdata))
    {
        UBF_LOG(log_error, "%s: failed to Bprint %p buffer: %s", 
                __func__, cdata, Bstrerror(Berror));
        ndrxj_ubf_throw(env, Berror, "%s: failed to Bprint %p buffer: %s", 
                __func__, cdata, Bstrerror(Berror));
        goto out;
    }
    
out:
    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);
}

/**
 * Control data for compiled boolean expression printing
 */
typedef struct Bextread_ctl Bextread_ctl_t;
struct Bextread_ctl
{
    JNIEnv * env;
    jobject data;
    jobject reader;
};

/**
 * Read data line from BufferedReader
 * @param buffer output buffer
 * @param bufsz output buffer size
 * @param dataptr1 user specific data ptr, holds java context
 * @return number bytes read
 */
exprivate long Bextread_readf(char *buffer, long bufsz, void *dataptr1)
{
    Bextread_ctl_t *ctl = (Bextread_ctl_t *)dataptr1;
    jclass clazz;
    jmethodID mid;
    long ret = EXSUCCEED;
    TPCONTEXT_T context;
    jstring str;
    jboolean n_str_copy = EXFALSE;
    const char *n_str;
    
    clazz = (*(ctl->env))->GetObjectClass(ctl->env, ctl->reader);
    
    if (NULL==clazz)
    {
        NDRX_LOG(log_error, "%s: Failed to get Buffered reader class",
                __func__);
        EXFAIL_OUT(ret);
    }
    
    mid = (*(ctl->env))->GetMethodID(ctl->env, clazz, "readLine",
            "()Ljava/lang/String;");
    
    if (NULL==mid)
    {
        NDRX_LOG(log_error, "%s: Failed to get readLine() method!",
                __func__);
        EXFAIL_OUT(ret);
    }

    /* suspend ATMI context as java might perform some other actions
     * on given thread.
     */
    tpgetctxt(&context, 0L);
    /* Call server object */
    str = (*(ctl->env))->CallObjectMethod(ctl->env, ctl->reader, mid);
    /* restore ATMI context */
    tpsetctxt(context, 0L);
    
    if (NULL==str)
    {
        if((*(ctl->env))->ExceptionCheck(ctl->env))
        {
            
            NDRXJ_LOG_EXCEPTION((ctl->env), log_error, NDRXJ_LOGEX_UBF, 
                    "Failed to buffered read: %s");
            
            EXFAIL_OUT(ret);
        }
        
        UBF_LOG(log_debug, "EOF reached on buffered reader");
        ret = 0;
        goto out;
    }
    
    /* transfer bytes to buffer */
    n_str= (*(ctl->env))->GetStringUTFChars(ctl->env, str, &n_str_copy);
     
    NDRX_STRNCPY_SAFE(buffer, n_str, bufsz);
    ret = strlen(buffer)+1;

out:

    return ret;
}

/**
 * Read UBF buffer from java input stream
 * @param env Java env
 * @param data UBF buffer
 * @param reader buffered input reader
 */
expublic void JNICALL Java_org_endurox_TypedUbf_Bextread
  (JNIEnv * env, jobject data, jobject reader)
{
    char *cdata;
    long clen;
    Bextread_ctl_t ctl;
    
    /* get the context, switch */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
       return; 
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, &clen))
    {
        UBF_LOG(log_error, "Failed to get buffer data");
        goto out;
    }
    
    /* read the buffer from interface */
    ctl.env = env;
    ctl.data = data;
    ctl.reader = reader;

    if (EXSUCCEED!=Bextreadcb((UBFH *)cdata, Bextread_readf, (void *)&ctl))
    {
        UBF_LOG(log_error, "Failed to call Bextreadcb(): %s", Bstrerror(Berror));
        ndrxj_ubf_throw(env, Berror, "Failed to call Bextreadcb(): %s", 
                Bstrerror(Berror));
        goto out;
    }
    
out:
    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);
}


/* vim: set ts=4 sw=4 et cindent: */


