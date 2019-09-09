/**
 * @brief Java UBF Backing routines
 *
 * @file TypedUbf_Bbool.c
 */
/* -----------------------------------------------------------------------------
 * Enduro/X Middleware Platform for Distributed Transaction Processing
 * Copyright (C) 2009-2016, ATR Baltic, Ltd. All Rights Reserved.
 * Copyright (C) 2017-2019, Mavimax, Ltd. All Rights Reserved.
 * This software is released under one of the following licenses:
 * LGPL or Mavimax's license for commercial use.
 * See LICENSE file for full text.
 *
 * C (as designed by Dennis Ritchie and later authors) language code is licensed
 * under Enduro/X Modified GNU Affero General Public License, version 3.
 * See LICENSE_C file for full text.
 * -----------------------------------------------------------------------------
 * LGPL license:
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License, version 3 as published
 * by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU Lesser General Public License, version 3
 * for more details.
 *
 * You should have received a copy of the Lesser General Public License along 
 * with this program; if not, write to the Free Software Foundation, Inc., 
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
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
#include <exjglue.h>
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
    jstring jfuncname = NULL;
    
    /* suspend ATMI context as java might perform some other actions
     * on given thread.
     */
    tpgetctxt(&context, 0L);
    
    jfuncname = (*M_cb_env)->NewStringUTF(M_cb_env, funcname);
    
    if((*M_cb_env)->ExceptionCheck(M_cb_env))
    {
        NDRXJ_LOG_EXCEPTION(M_cb_env, log_error, NDRXJ_LOGEX_ULOG, 
                "Failed to allocate string: %s (funcname: %s)", funcname);
        EXFAIL_OUT(ret);
    }
    
    ret = (*M_cb_env)->CallLongMethod(M_cb_env, M_cb_ubf, 
            ndrxj_clazz_TypedUbf_mid_boolcbfDispatch, jfuncname);
    
out:
    
    if (NULL!=jfuncname)
    {
        (*M_cb_env)->DeleteLocalRef(M_cb_env, jfuncname);
    }
            
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
JNIEXPORT jboolean JNICALL ndrxj_Java_org_endurox_TypedUbf_Bboolev
  (JNIEnv * env, jobject data, jobject jexpr) 
{
    jboolean jret = JNI_FALSE;
    int ret = EXSUCCEED;
    
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
    if (NULL==(tree = ndrxj_BExprTree_ptr_get(env, jexpr)))
    {
        UBF_LOG(log_error, "Failed to get compiled expression ptr!");
        goto out;
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, &clen,
            NULL, EXFALSE, EXFALSE))
    {
        NDRX_LOG(log_error, "Failed to get buffer data");
        goto out;
    }
    
    /* evaluate */
    if (EXFAIL==(ret = Bboolev((UBFH *)cdata, tree)))
    {
        ndrxj_ubf_throw(M_cb_env, Berror, "Failed to execute Bboolev(): %s",
                Bstrerror(Berror));
        goto out;
    }
    
    if (EXTRUE==ret)
    {
        jret = JNI_TRUE;
    }
    else
    {
        jret = JNI_FALSE;
    }

out:
    
    /* unset context */
    tpsetctxt(TPNULLCONTEXT, 0L);

    return jret;
}

/**
 * Evaluate expression and return float result
 * @param env java env
 * @param data UBF buffer
 * @param jexpr compiled boolean expression
 * @return  evaluated float value
 */
expublic jdouble JNICALL ndrxj_Java_org_endurox_TypedUbf_Bfloatev
  (JNIEnv *env, jobject data, jobject jexpr)
{
    jdouble jret = EXFAIL;
    
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
    if (NULL==(tree = ndrxj_BExprTree_ptr_get(env, jexpr)))
    {
        UBF_LOG(log_error, "Failed to get compiled expression ptr!");
        goto out;
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, &clen,
            NULL, EXFALSE, EXFALSE))
    {
        NDRX_LOG(log_error, "Failed to get buffer data");
        goto out;
    }
    
    /* evaluate */
    jret = (jdouble)Bfloatev((UBFH *)cdata, tree);
    
    if (BMINVAL!=Berror)
    {
        ndrxj_ubf_throw(M_cb_env, Berror, "Failed to execute Bfloatev(): %s",
                Bstrerror(Berror));
        goto out;
    }
    
out:
    
    /* unset context */
    tpsetctxt(TPNULLCONTEXT, 0L);

    return jret;
}

/**
 * Quick boolean expression evaluate
 * @param env java env
 * @param data UBF buffer
 * @param jexprstr boolean expression string
 */
expublic jboolean JNICALL ndrxj_Java_org_endurox_TypedUbf_Bqboolev
  (JNIEnv * env, jobject data, jstring jexprstr)
{
    jboolean jret = JNI_FALSE;
    int ret = EXSUCCEED;
    jboolean n_jexprstr_copy = EXFALSE;
    const char *n_jexprstr;
    
    char *tree = NULL;
    char *cdata;
    long clen;
    
    /* check arguments */
    if (NULL==jexprstr)
    {
        ndrxj_ubf_throw(env, Berror, "compiled expression must not be NULL: %s");
        goto out;
    }
    
    n_jexprstr = (*env)->GetStringUTFChars(env, jexprstr, &n_jexprstr_copy);
    
    /* save in thread vars the java environment data and UBF ptr */
    M_cb_env = env;
    M_cb_ubf = data;
    
    /* set context (from UBF buffer) */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
        goto out;
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, &clen,
            NULL, EXFALSE, EXFALSE))
    {
        NDRX_LOG(log_error, "Failed to get buffer data");
        goto out;
    }
    
    /* Compile the expression: */
    
    tree = Bboolco((char *)n_jexprstr);
    
    if (NULL==tree)
    {
        UBF_LOG(log_error, "Failed to compile: %s", Bstrerror(Berror));
        ndrxj_ubf_throw(env, Berror, "Failed to compile: %s", 
                Bstrerror(Berror));
        goto out;
    }

    /* Evaluate the expression: */
    
    if (EXFAIL==(ret = Bboolev((UBFH *)cdata, tree)))
    {
        ndrxj_ubf_throw(M_cb_env, Berror, "Failed to execute Bboolev(): %s",
                Bstrerror(Berror));
        goto out;
    }
    
    
    if (EXTRUE==ret)
    {
        jret = JNI_TRUE;
    }
    else
    {
        jret = JNI_FALSE;
    }
    
out:
    
    if (NULL!=tree)
    {
        Btreefree(tree);
    }

    if (n_jexprstr_copy)
    {
        (*env)->ReleaseStringUTFChars(env, jexprstr, n_jexprstr);
    }
    

    /* unset context */
    tpsetctxt(TPNULLCONTEXT, 0L);
            
    return jret;
}

/* vim: set ts=4 sw=4 et smartindent: */
