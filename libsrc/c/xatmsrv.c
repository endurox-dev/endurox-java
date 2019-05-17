/**
 * @brief XA support routines for Enduro/X Transaction Manager Server (tmsrv)
 *
 * @file xatmsrv.c
 */
/* -----------------------------------------------------------------------------
 * Enduro/X Middleware Platform for Distributed Transaction Processing
 * Copyright (C) 2009-2016, ATR Baltic, Ltd. All Rights Reserved.
 * Copyright (C) 2017-2018, Mavimax, Ltd. All Rights Reserved.
 * This software is released under one of the following licenses:
 * AGPL or Mavimax's license for commercial use.
 * -----------------------------------------------------------------------------
 * AGPL license:
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License, version 3 as published
 * by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU Affero General Public License, version 3
 * for more details.
 *
 * You should have received a copy of the GNU Affero General Public License along 
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
    
    /* call AtmiCtx factory method,
     * also prepare context for current thread
     * will get the env handler too.
     
    expublic int ndrxj_ldr_get_static_handler(JNIEnv *env, 
			char *run_class_str,
                        char *static_method,
                        char *static_method_sign,
                        jclass *run_class,
                        jmethodID *run_mid
  			)
    */
    ret = (int)(*M_jvm)->AttachCurrentThread(M_jvm, &env, NULL);
    
    if (ret!=JNI_OK)
    {
        NDRX_LOG(log_error, "Failed to attach/get env from java for current thread: %d", ret);
        EXFAIL_OUT(ret);
    }
    
    /* store the env, even if we have one, just common source.. */
    NDRXJ_JENV_LVAL(ctxpriv) = env;
    /*
    ret = ndrxj_ldr_get_static_handler(env, 
			char *run_class_str,
                        char *static_method,
                        char *static_method_sign,
                        jclass *run_class,
                        jmethodID *run_mid
  			)
     * */
    
    
    /* TODO: call  ndrxj_ldr_get_static_handler*/
    
#if 0
    
    jobject jret = NULL;
    jclass bclz = NULL;
    jmethodID mid;
    TPCONTEXT_T ctx;
    
    bclz = (*NDRXJ_JENV(ctxpriv))->FindClass(NDRXJ_JENV(ctxpriv), ATMICTX_CLASS);
    
    if (NULL==bclz)
    {        
        /* I guess we need to abort here! */
        NDRX_LOG(log_error, "Failed to find AtmiCtx - aborting...!");
        /* tpreturn fail or simulate time-out? or just abort?*/
        abort();
    }
    
        /* create buffer object... */
    mid = (*NDRXJ_JENV(ctxpriv))->GetMethodID(NDRXJ_JENV(ctxpriv), bclz, "<init>", "(J)V");
    
    if (NULL==mid)
    {
        NDRX_LOG(log_error, "Cannot get buffer constructor!");
        goto out;
    }

    NDRX_LOG(log_debug, "About to NewObject(%s)", ATMICTX_CLASS);
    
    /* Install current context handler */
    
    /* get current atmi context */
    tpgetctxt(&ctx, 0L);
    tpsetctxt(ctx, 0L);
    
    jret = (*NDRXJ_JENV(ctxpriv))->NewObject(NDRXJ_JENV(ctxpriv), bclz, mid, (jlong)ctx);
    
    if (NULL==jret)
    {
        NDRX_LOG(log_error, "Failed to create [%s]", ATMICTX_CLASS);
        
        /* backtrace? */
        NDRXJ_LOG_EXCEPTION((NDRXJ_JENV(ctxpriv)), log_error, NDRXJ_LOGEX_ULOG, 
                "Failed to create Atmi Context: %s");
        
        goto out;
    }
    
    NDRX_LOG(log_debug, "NewObject() done - atmi ctx");
    
    /* create reference... */
    jret = (*NDRXJ_JENV(ctxpriv))->NewGlobalRef(NDRXJ_JENV(ctxpriv), jret);
    
    NDRXJ_CCTX_LVAL(ctxpriv) = ctx;
    NDRXJ_JATMICTX_LVAL(ctxpriv) = jret;
    
#endif

out:
    
#if 0
    if (NULL!=bclz)
    {
        (*NDRXJ_JENV(ctxpriv))->DeleteLocalRef(NDRXJ_JENV(ctxpriv), bclz);
    }
#endif

    return ret;
}

/* vim: set ts=4 sw=4 et smartindent: */
