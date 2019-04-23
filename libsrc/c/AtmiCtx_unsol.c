/**
 * @brief Unsolicited message handling
 *
 * @file AtmiCtx_unsol.c
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

/*---------------------------Includes-----------------------------------*/
#include <jni.h>
#include <errno.h>
#include <stdlib.h>
#include "org_endurox_AtmiCtx.h"
#include <atmi.h>
#include <oatmi.h>
#include <ndebug.h>
#include <ondebug.h>
#include <oatmisrv_integra.h>
#include "libsrc.h"
#include <sys_unix.h>
#include <tpadm.h>
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/

/*
exprivate __thread JNIEnv* M_env;
exprivate __thread jobject M_atmiCtxObj;
 */

/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/

/**
 * Unsolicited message dispatcher.
 * We enter at ATMI context.
 * @param data incoming data buffer
 * @param len data buffer len
 * @param flags ?
 */
exprivate void ndrx_unsol_dispatcher (char *data, long len, long flags) 
{
    int ret = EXSUCCEED;
    jclass bclz;
    jmethodID mid;
    TPCONTEXT_T ctx;
    ndrx_ctx_priv_t *ctxpriv;
    
    ctxpriv = ndrx_ctx_priv_get();
    
    /* here we will need a thread local java env handler and atmi context 
     * while performing C call from java 
     */
    
    jobject jdata = ndrxj_atmi_TypedBuffer_translate(NDRXJ_JENV(ctxpriv), 
            NDRXJ_JATMICTX(ctxpriv), EXTRUE, data, len, NULL, NULL, EXFALSE);
    
    /* check if have exception..., then do not continue... */
    if ((*NDRXJ_JENV(ctxpriv))->ExceptionCheck(NDRXJ_JENV(ctxpriv)))
    {
        /* log exception, unset and continue... */
        EXFAIL_OUT(ret);
    }
    
    /* Call method from Atmi Context, before that unset the C
     * context, because we do not know in what threads java does works.
     */
    bclz = (*NDRXJ_JENV(ctxpriv))->FindClass(NDRXJ_JENV(ctxpriv), "org/endurox/AtmiCtx");

    if (NULL==bclz)
    {        
       /* I guess we need to abort here! */
       NDRX_LOG(log_error, "Failed to find AtmiCtx - aborting...!");
       /* tpreturn fail or simulate time-out? or just abort?*/
       abort();
    }

    /* create buffer object... */
    mid = (*NDRXJ_JENV(ctxpriv))->GetMethodID(NDRXJ_JENV(ctxpriv), bclz, 
           "unsolDispatch", "(Lorg/endurox/TypedBuffer;J)V");

    if (NULL==mid)
    {
       NDRX_LOG(log_error, "Failed to get unsolDispatch() method!");
       abort();
    }
    
    /* get the callback dispatcher */
    
    /* unset context */
    NDRX_LOG(log_debug, "About to invoke unsol hanlder...");
    
    tpgetctxt(&ctx, 0L);

    (*NDRXJ_JENV(ctxpriv))->CallVoidMethod(NDRXJ_JENV(ctxpriv), NDRXJ_JATMICTX(ctxpriv), 
            mid, jdata, (jlong)flags);

    /* set context back... */
    tpsetctxt(ctx, 0L);

    NDRX_LOG(log_debug, "After unsol handler...");
    
out:
    
    /* clear any exception if have one */
    
    if ((*NDRXJ_JENV(ctxpriv))->ExceptionCheck(NDRXJ_JENV(ctxpriv)))
    {
        /* log exception, unset and continue... */
        NDRXJ_LOG_EXCEPTION(NDRXJ_JENV(ctxpriv), log_error, NDRXJ_LOGEX_TP, 
                "Got exception during unsol handling (ignore): %s");
        (*NDRXJ_JENV(ctxpriv))->ExceptionClear(NDRXJ_JENV(ctxpriv));
    }

    /* delete any references.. */

    if (NULL!=jdata)
    {
        (*NDRXJ_JENV(ctxpriv))->DeleteLocalRef(NDRXJ_JENV(ctxpriv), jdata);
    }

    if (NULL!=bclz)
    {
        (*NDRXJ_JENV(ctxpriv))->DeleteLocalRef(NDRXJ_JENV(ctxpriv), bclz);
    }

    return;
}

/**
 * Set unsolicited callback handler
 * @param env java env
 * @param atmiCtxObj atmi context object
 * @param jcb java object for unsol callback
 * @return curren unsol java hanlder
 */
JNIEXPORT void JNICALL Java_org_endurox_AtmiCtx_tpsetunsolC
  (JNIEnv * env, jobject atmiCtxObj, jobject jcb)
{
    TPCONTEXT_T ctx;
    int err;
    
    if (NULL==(ctx = ndrxj_get_ctx(env, atmiCtxObj, EXTRUE)))
    {
        return;
    }
    
    /* Register callback */    
    if (NULL!=jcb)
    {
        /* register java callback for unsol handler */
        if (TPUNSOLERR==tpsetunsol(ndrx_unsol_dispatcher))
        {
            err = tperrno;
            /* generate exception... */
            ndrxj_atmi_throw(env, NULL, NULL, err, tpstrerror(err));
            goto out;
        }
    }
    else 
    {
        if (TPUNSOLERR==tpsetunsol(NULL))
        {
            err = tperrno;
            /* generate exception... */
            ndrxj_atmi_throw(env, NULL, NULL, err, tpstrerror(err));
            goto out;
        }
    }
    
out:

    tpsetctxt(TPNULLCONTEXT, 0L);   
}

/**
 * Perfrom notify call
 * @param env java env
 * @param atmiCtxObj Atmi context
 * @param jclientid target client id
 * @param idata input data buffer
 * @param flags flags
 */
JNIEXPORT void JNICALL Java_org_endurox_AtmiCtx_tpnotify
  (JNIEnv * env, jobject atmiCtxObj, jobject jclientid, jobject idata, jlong flags)
{
    int ret = EXSUCCEED;
    TPCONTEXT_T ctx;
    CLIENTID cltid;
    char *ibuf = NULL;
    long ilen = 0;
    int err;
    
    if (NULL==(ctx = ndrxj_get_ctx(env, atmiCtxObj, EXTRUE)))
    {
        return;
    }
    
    /* check client id */
    if (NULL==jclientid)
    {
        ndrxj_atmi_throw(env, idata, NULL, TPEINVAL, "jclientid is NULL");
    }
    
    /* restore client id */
    if (EXSUCCEED!=ndrxj_atmi_ClientId_translate_toc(env, 
        jclientid, &cltid))
    {
        if ((*env)->ExceptionCheck(env))
        {
            EXFAIL_OUT(ret);
        }
        else
        {
            ndrxj_atmi_throw(env, idata, NULL, TPESYSTEM, 
                    "Failed to translate jclientid to C");
            EXFAIL_OUT(ret);
        }
    }
    
    /* translate the buffer to c */
    
    /* get data buffer... */
    if (NULL!=idata)
    {
        if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, idata, &ibuf, &ilen, 
                NULL, EXFALSE, EXFALSE))
        {
            NDRX_LOG(log_error, "Failed to get data buffer!");
            goto out;
        }
    }
    
    /* notify the client with data buffer */
    if (EXSUCCEED!=tpnotify(&cltid, ibuf, ilen, (long)flags))
    {
        err = tperrno;
        
        NDRX_LOG(log_error, "Failed to notify client [%s]: %s", 
                cltid.clientdata, strerror(err));
        /* throw exception */
        
        ndrxj_atmi_throw(env, idata, NULL, err, tpstrerror(err));
    }
    
    out:
    
    tpsetctxt(TPNULLCONTEXT, 0L);
}

/**
 * tpbroadcast backend for java
 * @param env java env
 * @param atmiCtxObj ATMI Context object
 * @param lmid cluster node id/s (or regex mask)
 * @param usrname RFU
 * @param cltname client name/s (or regex mask)
 * @param idata input data buffer, may be NULL
 * @param flags flags
 */
JNIEXPORT void JNICALL Java_org_endurox_AtmiCtx_tpbroadcast
  (JNIEnv * env, jobject atmiCtxObj, jstring lmid, jstring usrname, 
        jstring cltname, jobject idata, jlong flags)
{
    int ret = EXSUCCEED;
    TPCONTEXT_T ctx;
    
    const char *n_lmid = NULL;
    jboolean n_lmid_copy = EXFALSE;
    
    const char *n_usrname = NULL;
    jboolean n_usrname_copy = EXFALSE;
    
    const char *n_cltname = NULL;
    jboolean n_cltname_copy = EXFALSE;
    
    char *ibuf = NULL;
    long ilen = 0;
    
    int err;

    /* switch to C */
    if (NULL==(ctx = ndrxj_get_ctx(env, atmiCtxObj, EXTRUE)))
    {
        return;
    }
    
    /* get data buffer to broadcast */
    
    if (NULL!=idata)
    {
        if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, idata, &ibuf, &ilen, 
                NULL, EXFALSE, EXFALSE))
        {
            NDRX_LOG(log_error, "Failed to get data buffer!");
            goto out;
        }
    }
   
    /* read the filter fields... */
    if (NULL!=lmid)
    {
        n_lmid  = (*env)->GetStringUTFChars(env, lmid, &n_lmid_copy);
    }
    
    if (NULL!=usrname)
    {
        n_usrname  = (*env)->GetStringUTFChars(env, usrname, &n_usrname_copy);
    }
    
    if (NULL!=cltname)
    {
        n_cltname  = (*env)->GetStringUTFChars(env, cltname, &n_cltname_copy);
    }
    
    if (EXSUCCEED!=tpbroadcast((char *)n_lmid, (char *)n_usrname, (char *)n_cltname, 
            (char *)ibuf, ilen, (long)flags))
    {
        err = tperrno;
        NDRX_LOG(log_error, "Failed to broadcast: %s", tpstrerror(err));
        
        /* generate exception... */
        ndrxj_atmi_throw(env, idata, NULL, err, tpstrerror(err));

    }
    
out:
    
    if (n_lmid_copy)
    {
        (*env)->ReleaseStringUTFChars(env, lmid, n_lmid);
    }

    if (n_usrname_copy)
    {
        (*env)->ReleaseStringUTFChars(env, usrname, n_usrname);
    }

    if (n_cltname_copy)
    {
        (*env)->ReleaseStringUTFChars(env, cltname, n_cltname);
    }

    tpsetctxt(TPNULLCONTEXT, 0L);

    return;
}

/**
 * Check for any unsolicated messages
 * @param env java env
 * @param atmiCtxObj ATMI Con
 * @return EXFAIL or 
 */
JNIEXPORT jint JNICALL Java_org_endurox_AtmiCtx_tpchkunsol
  (JNIEnv * env, jobject atmiCtxObj)
{
    int ret = EXSUCCEED;
    TPCONTEXT_T ctx;
    int err;

    /* switch to C */
    if (NULL==(ctx = ndrxj_get_ctx(env, atmiCtxObj, EXTRUE)))
    {
        return EXFAIL;
    }

    if (EXFAIL==(ret = tpchkunsol()))
    {
        err = tperrno;
        
        NDRX_LOG(log_error, "Failed to run tpchkunsol():  %s", 
                strerror(err));
        
        /* throw exception */
        ndrxj_atmi_throw(env, NULL, NULL, err, tpstrerror(err));
    }
    
out:

    tpsetctxt(TPNULLCONTEXT, 0L);

    return (jint)ret;
}


/* vim: set ts=4 sw=4 et smartindent: */
