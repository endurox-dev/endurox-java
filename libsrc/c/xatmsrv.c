/**
 * @brief XA support routines for Enduro/X Transaction Manager Server (tmsrv)
 *
 * @file xatmsrv.c
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
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <ctype.h>
#include <errno.h>

#include <userlog.h>

#include <ndrstandard.h>
#include <ndebug.h>

#include <exparson.h>
#include <jni.h>
#include <ubf.h>
#include <libsrc.h>
#include <exjldsys.h>
#include <tmenv.h>
/*------------------------------Externs---------------------------------------*/
/*------------------------------Macros----------------------------------------*/
#define ATMICTX_CLASS   "org/endurox/AtmiCtx"
/*------------------------------Enums-----------------------------------------*/
/*------------------------------Typedefs--------------------------------------*/
/*------------------------------Globals---------------------------------------*/
/*------------------------------Statics---------------------------------------*/

/**
 * Virtual machine handler started for C processes / not loaded by java
 */
exprivate JavaVM *M_jvm = NULL;

exprivate ndrxj_class_index_t M_dum[] = {
    {NULL}
};

/*------------------------------Prototypes------------------------------------*/

/**
 * Create VM.
 * Still no ATMI Context allocation. That will be done once actual XA calls
 * will be done.
 * The java env will be ignored.
 */
expublic int ndrxj_jvm_create(ndrx_ctx_priv_t *ctxpriv)
{
    int ret = EXSUCCEED;
    JNIEnv *env = NULL;
    M_jvm = ndrxj_ldr_getvm(M_dum, 0, M_dum, 0, &env);
    
    if (NULL==M_jvm)
    {
        NDRX_LOG(log_error, "Failed to create Java Virtula Machine");
        EXFAIL_OUT(ret);
    }
    
    NDRXJ_JENV_LVAL(ctxpriv) = env;
    
out:
    return ret;
}

/**
 * Allocate ATMI Context object and install it in ATMI C Context
 * @return 
 */
expublic int ndrxj_alloc_context(ndrx_ctx_priv_t *ctxpriv)
{
    int ret = EXSUCCEED;
    JNIEnv *env = NULL;
    jclass ctx_class = NULL;
    jmethodID factory_mid = NULL;
    TPCONTEXT_T ctx = NULL;
    jobject jctx = NULL;
    
    /* call AtmiCtx factory method,
     * also prepare context for current thread
     * will get the env handler too.
     */
    
    /* tmsrv threads will become manually allocated
     * otherwise at times will kill TLS due to auto flag
     */
    ndrx_ctx_auto(EXFALSE);
    
    ret=(int)(*M_jvm)->AttachCurrentThread(M_jvm, (void **)&env, NULL);
    
    if (ret!=JNI_OK)
    {
        NDRX_LOG(log_error, "Failed to attach/get env from java for current thread: %d", ret);
        EXFAIL_OUT(ret);
    }
    
    /* store the env, even if we have one, just common source.. */
    NDRXJ_JENV_LVAL(ctxpriv) = env;
    
    ret = ndrxj_ldr_get_static_handler(env, 
			"org.endurox.AtmiCtx",
                        "createAtmiCtx",
                        "(J)Lorg/endurox/AtmiCtx;",
                        &ctx_class,
                        &factory_mid);
    
    if (EXSUCCEED!=ret)
    {
        NDRX_LOG(log_error, "Failed to get createAtmiCtx() handler");
        EXFAIL_OUT(ret);
    }
    
    tpgetctxt(&ctx, 0L);
    tpsetctxt(ctx, 0L);
    /* save our C context */
    NDRXJ_CCTX_LVAL(ctxpriv) = ctx;
    
    /* Invoke the create at save the returned object 
     * object shall be created as global ref
     */
    NDRX_LOG(log_debug, "Create ATMI CTX with C context %p", ctx);
    
    jctx = (*env)->CallStaticObjectMethod(env, ctx_class, factory_mid, (jlong)(long)ctx);
    
    if((*env)->ExceptionCheck(env))
    {
        NDRXJ_LOG_EXCEPTION(NDRXJ_JENV(ctxpriv), log_error, 
            NDRXJ_LOGEX_NDRX, "Failed to create AtmiCtx obj:%s");
        
        (*env)->ExceptionClear(env);
        EXFAIL_OUT(ret);
    }
    
    if (NULL==jctx)
    {
        NDRX_LOG(log_error, "Failed to create ATMI Context!");
        EXFAIL_OUT(ret);
    }

    /* make global ref... */
    
    jctx=(*NDRXJ_JENV(ctxpriv))->NewGlobalRef(NDRXJ_JENV(ctxpriv), jctx);
    
    NDRXJ_JATMICTX_LVAL(ctxpriv) = jctx;
    
    /* set this as server thread... */
    NDRXJ_CTXFLAGS(ctxpriv) |= NDRXJ_CTXFLAGS_SRV;
    
    NDRX_LOG(log_info, "Atmi context allocated");
    
out:
    
    if (NULL!=ctx_class)
    {
        (*NDRXJ_JENV(ctxpriv))->DeleteLocalRef(NDRXJ_JENV(ctxpriv), ctx_class);
    }

    return ret;
}

/* vim: set ts=4 sw=4 et smartindent: */
