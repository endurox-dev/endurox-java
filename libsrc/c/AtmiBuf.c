/* 
** ATMI Context backing JNI functions
**
** @file AtmiCtx.c
** 
** -----------------------------------------------------------------------------
** Enduro/X Middleware Platform for Distributed Transaction Processing
** Copyright (C) 2015, Mavimax, Ltd. All Rights Reserved.
** This software is released under one of the following licenses:
** GPL or Mavimax's license for commercial use.
** -----------------------------------------------------------------------------
** GPL license:
** 
** This program is free software; you can redistribute it and/or modify it under
** the terms of the GNU General Public License as published by the Free Software
** Foundation; either version 2 of the License, or (at your option) any later
** version.
**
** This program is distributed in the hope that it will be useful, but WITHOUT ANY
** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
** PARTICULAR PURPOSE. See the GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License along with
** this program; if not, write to the Free Software Foundation, Inc., 59 Temple
** Place, Suite 330, Boston, MA 02111-1307 USA
**
** -----------------------------------------------------------------------------
** A commercial use license is available from Mavimax, Ltd
** contact@mavimax.com
** -----------------------------------------------------------------------------
*/

/*---------------------------Includes-----------------------------------*/
#include <jni.h>
#include "org_endurox_AtmiBuf.h"
#include <atmi.h>
#include <oatmi.h>
#include <ndebug.h>
#include "libsrc.h"
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/

/**
 * Free up the the context
 * @param env java env
 * @param obj ATMI buffer Object
 * @param cPtr C pointer to ATMI Buffer
 */
void JNICALL Java_org_endurox_AtmiBuf_tpfree (JNIEnv *env, jobject obj, jlong cPtr)
{
    TPCONTEXT_T ctx;

    jclass objClass = (*env)->GetObjectClass(env, obj);
    jfieldID atmi_ctx_fld = (*env)->GetFieldID(env, objClass, "ctx", "Lorg/endurox/AtmiCtx;");
    jobject atmi_ctx_obj = (*env)->GetObjectField(env, obj, atmi_ctx_fld);
    
    if (NULL==(ctx=ndrxj_get_ctx(env, atmi_ctx_obj, EXTRUE)))
    {
        goto out;
    }

    NDRX_LOG(log_debug, "About to free up: context: %p buf: %p",
            ctx, (void *)cPtr);
    
    tpfree((char *)cPtr);
    
    /* unset context */
    tpsetctxt(TPNULLCONTEXT, 0L);

out:
    /* return object */
    return;
}

expublic jobject ndrxj_atmi_AtmiBuf_translate(JNIEnv *env, 
            jobject ctx_obj, int is_ctxset, char *data, long len)
{
    jobject ret = NULL;
    jclass bclz;
    jmethodID mid;
    jstring jclientdata;
    int we_set_ctx = EXFALSE;

    /* Set context if needed */
    if (!is_ctxset)
    {
        if (NULL==ndrxj_get_ctx(env, ctx_obj, EXTRUE))
        {
            goto out;
        }
        
        we_set_ctx = EXTRUE;
    }
    
    /* TODO: Re-use tpalloc's code */

out:
    
    if (we_set_ctx)
    {
        /* return back to NULL */
        tpsetctxt(TPNULLCONTEXT, 0L);
    }

    return ret;
}


/* vim: set ts=4 sw=4 et cindent: */