/**
 * @brief Java UBF Backing routines
 *
 * @file TypedUBF_Bprint.c
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
#include "org_endurox_TypedUBF.h"
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

/* These bellow are used by callback. Set by evaluator */

/** 
 * Java env for server operations 
 */
exprivate __thread JNIEnv *M_cb_env = NULL;

/**
 * This is current UBF buffer running for the expressions
 */
exprivate __thread jobject M_cb_ubf;


/*---------------------------Prototypes---------------------------------*/

/**
 * Print the UBF buffer to STDOUT
 * @param env java env
 * @param data TypedUBF object
 */
expublic void JNICALL Java_org_endurox_TypedUBF_Bprint(JNIEnv *env, jobject data)
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
        NDRX_LOG(log_error, "Failed to get buffer data");
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
 * When performing eval on UBF buffer, we must save in current thread
 * local stored variable an object pointer to UBF buffer.
 * @param p_ub
 * @param funcname
 * @return long value returned from function
 */
expublic long Bbool_callback_function(UBFH *p_ub, char *funcname)
{
    long ret = EXSUCCEED;
    TPCONTEXT_T context;
    jclass clazz;
    jmethodID mid;
    jstring jfuncname;
    
    /* suspend ATMI context as java might perform some other actions
     * on given thread.
     */
    tpgetctxt(&context, 0L);
    
    
    /* locate write method of output stream class */
    
    clazz = (*M_cb_env)->GetObjectClass(M_cb_env, M_cb_ubf);
    
    if (NULL==clazz)
    {
        UBF_LOG(log_error, "%s: Failed to get UBF buffer class",
                __func__);
        /* we shall throw exception here! */
        
        ndrxj_ubf_throw(M_cb_env, NULL, BEUNIX, "%s: Failed to get UBF buffer class",
                __func__);
        
        EXFAIL_OUT(ret);
    }
    
    /* get dispatch callback */
    
    mid = (*M_cb_env)->GetMethodID(M_cb_env, clazz, "boolcbfDispatch",
            "(Ljava/lang/String;)J");
    
    if (NULL==mid)
    {
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_ULOG, 
                "Failed to get boolcbfDispatch method: %s");
        EXFAIL_OUT(ret);
    }
    
    jfuncname = (*M_cb_env)->NewStringUTF(M_cb_env, funcname);
    
    if((*M_cb_env)->ExceptionCheck(M_cb_env))
    {
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_ULOG, 
                "Failed to allocate string: %s (funcname: %s)", funcname);
        EXFAIL_OUT(ret);
    }
    
    ret = (*M_cb_env)->CallLongMethod(M_cb_env, M_cb_ubf, mid, jfuncname);
    
out:
    
    /* restore ATMI context */
    tpsetctxt(context, 0L);

    return ret;
}

/**
 * Evaluate boolean expression
 * @param env java env
 * @param data UBF buffer
 * @param jexpr compiled boolean expression
 * @return JNI_TRUE, JNI_FALSE
 */
JNIEXPORT jboolean JNICALL Java_org_endurox_TypedUBF_Bboolev
  (JNIEnv * env, jobject data, jobject jexpr) {
    
    jboolean ret = JNI_FALSE;
    char *tree;
    char *cdata;
    long clen;
    
    /* check arguments */
    
    if (NULL==jexpr)
    {
        ndrxj_ubf_throw(env, Berror, "compiled expression must not be NULL: %s");
        goto out;
    }
    
    /* save in thread vars the java environment data and UBF ptr */
    M_cb_env = env;
    M_cb_ubf = data;
    
    
    /* set context (from UBF buffer) */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
        goto out;
    }
    
    /* get expression ptr */
    if (NULL==(tree = ndrxj_BExprTree_ptr_get(JNIEnv *env, jexpr)))
    {
        UBF_LOG(log_error, "Failed to get compiled expression ptr!");
        goto out;
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, &clen))
    {
        NDRX_LOG(log_error, "Failed to get buffer data");
        goto out;
    }
    
    /* evaluate */
    ret = Bboolev((UBFH *)data, tree);

out:
    
    /* unset context */
    tpsetctxt(TPNULLCONTEXT, 0L);

    return (jboolean)ret;
}

/* vim: set ts=4 sw=4 et cindent: */
