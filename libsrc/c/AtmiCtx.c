/**
 * @brief ATMI Context backing JNI functions
 *
 * @file AtmiCtx.c
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
#include "exjglue.h"
#include <atmi.h>
#include <oatmi.h>
#include <ndebug.h>
#include <ondebug.h>
#include <oatmisrv_integra.h>
#include <libsrc.h>
#include <sys_unix.h>
#include <tmenv.h>
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/

/* NOTE: we can have single main thread only, thus we may use globals */

/** Java env for server operations */
/* exprivate JNIEnv *NDRXJ_JENV(ctxpriv) = NULL; */

/** Context object */
/* exprivate jobject NDRXJ_JATMICTX(ctxpriv) = NULL; */

/** Resolve context */
/* exprivate TPCONTEXT_T M_srv_ctx = NULL; */

/** Command line arguments as passed to the server runner */
exprivate jobjectArray M_jargv;

/*---------------------------Prototypes---------------------------------*/

/**
 * get context from current object
 * TODO: Extend to get context from wrapper object which have a reference
 * to context object (OPTIONAL)
 * @param env java env
 * @param atmiCtxObj ATMI Context object
 * @return NULL (and expection set) or context data
 */
expublic TPCONTEXT_T ndrxj_get_ctx(JNIEnv *env, jobject atmiCtxObj, int do_set)
{
    TPCONTEXT_T ctx;
    
    jlong fieldVal = (*env)->GetLongField(env, atmiCtxObj, ndrxj_clazz_AtmiCtx_fid_ctx);

    ctx = (TPCONTEXT_T)fieldVal;

    if (NULL==ctx)
    {
        ndrxj_atmi_throw(env, NULL, NULL, TPEINVAL, 
                "NULL C context for ATMI Context OP!");
    }
    else
    {
        if (do_set)
        {
            ndrx_ctx_priv_t *ctxpriv;

            tpsetctxt(ctx, 0L);
            
            ctxpriv = ndrx_ctx_priv_get();
            
            /* update private data storage in context */
            NDRXJ_JENV_LVAL(ctxpriv) = env;
            
            /* why it changes ptr??? because function receives local reference
             * So for client processes we do not have global ref?
             * Only in case of server process we shall not override the context
             * we have passed in.
             * 
             * Context is stored only for servers.
             */
            if (!(NDRXJ_CTXFLAGS(ctxpriv) & NDRXJ_CTXFLAGS_SRV))
            {
                /* as it is used by client, it will be made free when returning
                 * back to java
                 * Problem is that in case of XA:
                 * - Java goes to C
                 * - C goes back to Java
                 * ---> Here atmiCtxObj reference is changed
                 * - returns to C
                 * Do we need stack if:
                 * - Java goes to C
                 * - C goes to Java
                 * - Java goes to C
                 * - C returns from java (removes global ref..) ?
                 * 
                 */
                
                /* Check do we have valid reference here?
                 * if not then free it up and allocate new one
                 */
                if (NULL!=NDRXJ_JATMICTX_LVAL(ctxpriv) && 
                        !(*env)->IsSameObject(env, NDRXJ_JATMICTX_LVAL(ctxpriv), atmiCtxObj))
                {
                    (*env)->DeleteWeakGlobalRef(env, NDRXJ_JATMICTX_LVAL(ctxpriv));
                    
                    NDRXJ_JATMICTX_LVAL(ctxpriv) = (*env)->NewWeakGlobalRef(env, atmiCtxObj);
                    
                    /* Dealloc is done with C side finalizerC() when AtmiCtx
                     * object is destructed.
                     */
                }
                
                if (NULL==NDRXJ_JATMICTX_LVAL(ctxpriv))
                {
                    NDRXJ_JATMICTX_LVAL(ctxpriv) = (*env)->NewWeakGlobalRef(env, atmiCtxObj);
                }
                
                NDRXJ_CCTX_LVAL(ctxpriv) = ctx;
                
            }
        }
    }
    
    return ctx;
}

/**
 * Query log information
 * @param env java env
 * @param obj Atmi context
 * @param lev log level
 * @param flags flags
 * @return logger configuration
 */
expublic jint JNICALL ndrxj_Java_org_endurox_AtmiCtx_tplogqinfo (JNIEnv *env, jobject obj, 
        jint lev, jlong flags)
{
    TPCONTEXT_T ctx;
    int ret = 0;
    
    if (NULL==(ctx = ndrxj_get_ctx(env, obj, EXTRUE)))
    {
        goto out;
    }
    
    ret = tplogqinfo((int)lev, (long)flags);
    
    if (ret < 0)
    {
        /* throw exception */
        ndrxj_nstd_throw(env, Nerror, Nstrerror(Nerror));
    }
    
out:
    tpsetctxt(TPNULLCONTEXT, 0L);
    return ret;
}

/**
 * TP Log entry
 * @param env java nev
 * @param obj java object (Atmi Context)
 * @param lev log level
 * @param file log file or empty string if not detailed log user
 * @param line line number of -1 if no detailed log user
 * @param msg message to log
 */
void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tplogC(JNIEnv * env, jobject obj, jint lev, 
        jstring file, jlong line, jstring msg)
{
    TPCONTEXT_T ctx;
    jboolean n_file_copy = EXFALSE;
    jboolean n_msg_copy = EXFALSE;
    
    /* TODO: file name for some reasons may be NULL */
    
    const char *n_file = "?.java";
    const char *n_msg = (*env)->GetStringUTFChars(env, msg, &n_msg_copy);
    
    if (NULL!=file)
    {
        n_file = (*env)->GetStringUTFChars(env, file, &n_file_copy);
    }
    
    /* check mandatory arguments...!, thow NullPointerException if any bad.. */
        
    if (NULL==(ctx = ndrxj_get_ctx(env, obj, EXTRUE)))
    {
        return;
    }
    
    if (line!=EXFAIL)
    {
        tplogex((int)lev, (char *)n_file, (long)line, (char *)n_msg);
    }
    else
    {
        tplog((int)lev, (char *)n_msg);
    }
    
out:
    
    tpsetctxt(TPNULLCONTEXT, 0L);

    if (n_file_copy)
    {
        (*env)->ReleaseStringUTFChars(env, file, n_file);
    }

    if (n_msg_copy)
    {
        (*env)->ReleaseStringUTFChars(env, msg, n_msg);
    }
}

/**
 * NDRX Log entry
 * @param env java nev
 * @param obj java object (Atmi Context)
 * @param lev log level
 * @param file log file or empty string if not detailed log user
 * @param line line number of -1 if no detailed log user
 * @param msg message to log
 */
void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tplogndrxC(JNIEnv * env, jobject obj, jint lev, 
        jstring file, jlong line, jstring msg)
{
    TPCONTEXT_T ctx;
    jboolean n_file_copy = EXFALSE;
    jboolean n_msg_copy = EXFALSE;
    
    const char *n_file = (*env)->GetStringUTFChars(env, file, &n_file_copy);
    const char *n_msg = (*env)->GetStringUTFChars(env, msg, &n_msg_copy);
    
    if (NULL==(ctx = ndrxj_get_ctx(env, obj, EXFALSE)))
    {
        return;
    }
    
    if (line!=EXFAIL)
    {
        Otplogex(&ctx, (int)lev, (char *)n_file, (long)line, (char *)n_msg);
    }
    else
    {
        Otplog(&ctx, (int)lev, (char *)n_msg);
    }
    
out:
    
    tpsetctxt(TPNULLCONTEXT, 0L);

    if (n_file_copy)
    {
        (*env)->ReleaseStringUTFChars(env, file, n_file);
    }

    if (n_msg_copy)
    {
        (*env)->ReleaseStringUTFChars(env, msg, n_msg);
    }
}

/**
 * Print hex dump to the log
 * @param env Java env
 * @param atmiCtxObj ATMI Context obj
 * @param lev debug level
 * @param msg message to print
 * @param data buffer to dump
 */
extern void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tplogDump
  (JNIEnv * env, jobject atmiCtxObj, jint lev, jstring msg, jbyteArray data)
{
    TPCONTEXT_T ctx;
    char * n_carray = NULL;
    jboolean n_msg_copy = EXFALSE;
    jboolean n_carray_copy = EXFALSE;
    jsize len;
    const char *n_msg = (*env)->GetStringUTFChars(env, msg, &n_msg_copy);
    n_carray = (char*)(*env)->GetByteArrayElements(env, data, &n_carray_copy);
    
    if (NULL==(ctx = ndrxj_get_ctx(env, atmiCtxObj, EXTRUE)))
    {
        return;
    }
    
    len = (*env)->GetArrayLength(env, data);
    
    tplogdump(lev, (char *)n_msg, n_carray, len);
    
out:
    
    tpsetctxt(TPNULLCONTEXT, 0L);

    if (n_msg_copy)
    {
        (*env)->ReleaseStringUTFChars(env, msg, n_msg);
    }

    if(n_carray_copy)
    {
       (*env)->ReleaseByteArrayElements(env, data, n_carray, JNI_ABORT);
    }
}

/**
 * Print the byte array differences to log file.
 * @param env java env
 * @param atmiCtxObj Atmi Context obj
 * @param lev log level
 * @param msg debug message
 * @param data1 buffer 1
 * @param data2 buffer 2 to compare
 */
extern void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tplogDumpDiff
  (JNIEnv * env, jobject atmiCtxObj, jint lev, jstring msg, jbyteArray data1, jbyteArray data2)
{
    TPCONTEXT_T ctx;

    
    char * n_carray1 = NULL;
    jboolean n_carray_copy1 = EXFALSE;
    
    char * n_carray2 = NULL;
    jboolean n_carray_copy2 = EXFALSE;
    
    jsize len1;
    jsize len2;
    jboolean n_msg_copy = EXFALSE;
    const char *n_msg = (*env)->GetStringUTFChars(env, msg, &n_msg_copy);
    
    n_carray1 = (char*)(*env)->GetByteArrayElements(env, data1, &n_carray_copy1);
    n_carray2 = (char*)(*env)->GetByteArrayElements(env, data2, &n_carray_copy2);
    
    if (NULL==(ctx = ndrxj_get_ctx(env, atmiCtxObj, EXTRUE)))
    {
        return;
    }
    
    len1 = (*env)->GetArrayLength(env, data1);
    len2 = (*env)->GetArrayLength(env, data1);
    
    tplogdumpdiff(lev, (char *)n_msg, n_carray1, n_carray2, NDRX_MIN(len1, len2));
    
out:
    
    tpsetctxt(TPNULLCONTEXT, 0L);

    if (n_msg_copy)
    {
        (*env)->ReleaseStringUTFChars(env, msg, n_msg);
    }

    if(n_carray_copy1)
    {
       (*env)->ReleaseByteArrayElements(env, data1, n_carray1, JNI_ABORT);
    }

    if(n_carray_copy2)
    {
       (*env)->ReleaseByteArrayElements(env, data1, n_carray2, JNI_ABORT);
    }
}

/**
 * Free up the atmi context
 * @param env java env
 * @param cls class on which method is called (Atmi Context this case), static
 * @param cPtr pointer to context C object 
 */
expublic void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpfreectxt(JNIEnv *env, 
        jclass cls, jlong cPtr)
{
    TPCONTEXT_T ctx = (TPCONTEXT_T)cPtr;
    
    tpfreectxt(ctx);
}

/*
 * Class:     org_endurox_AtmiCtx
 * Method:    tpalloc
 * Signature: (Ljava/lang/String;Ljava/lang/String;J)Lorg/endurox/TypedBuffer;
 */
expublic jobject JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpalloc (JNIEnv *env, jobject obj, 
        jstring btype, jstring bsubtype, jlong size)
{
    jobject ret = NULL;
    char *data;
    TPCONTEXT_T ctx;
    jboolean n_btype_copy = EXFALSE;
    const char *n_btype;
    
    jboolean n_bsubtype_copy = EXFALSE;
    char *n_bsubtype = NULL;
    
    if (NULL!=bsubtype)
    {
        n_bsubtype = (char *)(*env)->GetStringUTFChars(env, bsubtype, &n_bsubtype_copy);
    }

    if (NULL==btype)
    {
        ndrxj_atmi_throw(env, NULL, NULL, TPEINVAL,
                "`type' is mandatory for tpAlloc, but got NULL!");
        goto out;
    }

    n_btype = (*env)->GetStringUTFChars(env, btype, &n_btype_copy);
    if (0==strcmp(n_btype, "NULL"))
    {
        /* NULL buffer is null buffer... */
        goto out;
    }

    /* get context handler */

    /* exception will thown if invalid object... */
    if (NULL==(ctx = ndrxj_get_ctx(env, obj, EXTRUE)))
    {
        goto out;
    }

    /* allocate buffer, if error throw exception  */
    data = tpalloc((char *)n_btype, (char *)n_bsubtype, (long)size);

    if (NULL==data)
    {
        int err = tperrno;
        /* Generate exception! */
        ndrxj_atmi_throw(env, NULL, NULL, err, tpstrerror(err));
        goto out;
    }
    
    /* Translate the handler to Java side */
    ret = ndrxj_atmi_TypedBuffer_translate(env,  obj, EXTRUE, data, size, 
            (char *)n_btype, (char *)n_bsubtype, EXTRUE);
    
out:
    /* unset context */
    tpsetctxt(TPNULLCONTEXT, 0L);

    if (n_btype_copy)
    {
        (*env)->ReleaseStringUTFChars(env, btype, n_btype);
    }

    if (n_bsubtype_copy)
    {
        (*env)->ReleaseStringUTFChars(env, bsubtype, n_bsubtype);
    }

    return ret;
}


/**
 * Class:     HelloJNI
 * Method:    sayHello
 * Signature: ()V
 */
jlong JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpnewctxt (JNIEnv *env, jclass cls)
{
    TPCONTEXT_T ctx = tpnewctxt(0, 1);

    if (NULL==ctx)
    {
        ndrxj_atmi_throw(env, NULL, NULL, TPESYSTEM, "Failed to allocate new ATMI context!");
    }
    else
    {
        NDRX_LOG(log_debug, "New ATMI context: %p", ctx);
        /* unset */
        tpsetctxt(TPNULLCONTEXT, 0L);
    }
    
    return (long)ctx;
}

/**
 * Get ATMI Error result
 */
JNIEXPORT jobject JNICALL ndrxj_Java_org_endurox_AtmiCtx_getAtmiError (JNIEnv *env, jobject obj)
{
    TPCONTEXT_T ctx;
    int err;
    jstring jstr;
    jobject errObj = NULL;
    
    /* exception will thrown if invalid object... */
    if (NULL==(ctx = ndrxj_get_ctx(env, obj, EXTRUE)))
    {
        return NULL;
    }

    NDRX_LOG(log_debug, "context: (%p)", ctx);
    err = tperrno;

    /* Create an instance of clazz */
    errObj = (*env)->NewObject(env, ndrxj_clazz_ErrorTuple, ndrxj_clazz_ErrorTuple_mid_INIT);

    /* Set fields for object */
    (*env)->SetIntField(env, errObj, ndrxj_clazz_ErrorTuple_fid_err, err);

    jstr=(jstring)((*env)->NewStringUTF(env, tpstrerror(err)) );
    (*env)->SetObjectField(env, errObj,ndrxj_clazz_ErrorTuple_fid_msg,(jobject)jstr);
    
    /* unset context */
    tpsetctxt(TPNULLCONTEXT, 0L);
    
out:
    /* return object */
    return errObj;
}

/**
 * Do the server init call back the interface
 * @param argc
 * @param argv
 * @return 
 */
exprivate int ndrxj_tpsvrinit(int argc, char ** argv)
{
    int ret = EXSUCCEED;    
    jobject svrObj = NULL;
    ndrx_ctx_priv_t *ctxpriv;
    
    ctxpriv = ndrx_ctx_priv_get();
    
    NDRX_LOG(log_info, "Into tpsrvinit -> java");
    
    
    svrObj = (*NDRXJ_JENV(ctxpriv))->GetObjectField(NDRXJ_JENV(ctxpriv), 
            NDRXJ_JATMICTX(ctxpriv), ndrxj_clazz_AtmiCtx_fid_svr);
    
    if (NULL==svrObj)
    {
        NDRX_LOG(log_error, "%s: Failed to get server interface object value!",
                __func__);
        EXFAIL_OUT(ret);
    }
    
    /* Now invoke the interface method */
 
    NDRX_LOG(log_debug, "About to call server interface...");

    /* Call server object */
    
    /* We shall enter into NULL context, not? */
    tpsetctxt(TPNULLCONTEXT, 0L);
    

    ret = (int)(*NDRXJ_JENV(ctxpriv))->CallIntMethod(NDRXJ_JENV(ctxpriv), svrObj, 
            ndrxj_clazz_Server_mid_tpSvrInit, NDRXJ_JATMICTX(ctxpriv), M_jargv);
    
    /* set back actual context */
    tpsetctxt(NDRXJ_CCTX(ctxpriv), 0L);
    
    /* check for exception, if have one the return  */
    
    if ((*NDRXJ_JENV(ctxpriv))->ExceptionCheck(NDRXJ_JENV(ctxpriv)))
    {
        NDRXJ_LOG_EXCEPTION(NDRXJ_JENV(ctxpriv), log_error, 
            NDRXJ_LOGEX_NDRX, "Java tpSvrInit failed:\n%s");
        (*NDRXJ_JENV(ctxpriv))->ExceptionClear(NDRXJ_JENV(ctxpriv));
        EXFAIL_OUT(ret);
    }
    
out:

    return ret;
}

/**
 * Dispatch call to Java side
 * SEE: https://stackoverflow.com/questions/12420463/keeping-a-global-reference-to-the-jnienv-environment
 * @param svcinfo 
 */
exprivate void dispatch_call(TPSVCINFO *svcinfo)
{
    /* build the svcinfo object and invoke the service proxy of java side */
    jobject jsvcinfo = NULL;
    jobject jdata = NULL;
    jobject jcltid = NULL;
    jstring jname = NULL;
    jstring jfname = NULL;
    ndrx_ctx_priv_t *ctxpriv;
    
    ctxpriv = ndrx_ctx_priv_get();
    
    if (NULL==(jsvcinfo = ndrxj_atmi_TpSvcInfo_translate(NDRXJ_JENV(ctxpriv),
            NDRXJ_JATMICTX(ctxpriv), EXTRUE, svcinfo, &jdata, &jcltid, &jname, &jfname)))
    {
        NDRX_LOG(log_error, "Failed to translate service call to java!");
    }
    else
    {
        NDRX_LOG(log_debug, "%s: Got java service info invoke service: [%s]", 
                __func__, svcinfo->name);

        /* unset context */
        tpsetctxt(TPNULLCONTEXT, 0L);
        
        (*NDRXJ_JENV(ctxpriv))->CallVoidMethod(NDRXJ_JENV(ctxpriv), NDRXJ_JATMICTX(ctxpriv), 
                ndrxj_clazz_AtmiCtx_mid_tpCallDispatch, jsvcinfo);
        
        /* set context back... */
        tpsetctxt(NDRXJ_CCTX(ctxpriv), 0L);
        
        /* Check exceptions, if have one, I guess we abort or return TPFAIL? 
         * If get get exception here, log down the output and abort the process
         * It is up to developer to handle the exceptions before doing
         * tpreturn...
         */
        if ((*NDRXJ_JENV(ctxpriv))->ExceptionCheck(NDRXJ_JENV(ctxpriv)))
        {
            NDRXJ_LOG_EXCEPTION(NDRXJ_JENV(ctxpriv), log_error, 
                NDRXJ_LOGEX_ULOG, "Service have thrown unexpected exception: "
                    "[%s] - ignoring (continue)");
            
            (*NDRXJ_JENV(ctxpriv))->ExceptionClear(NDRXJ_JENV(ctxpriv));
        }
    }

    /*
     * If not releasing allocated objects from C/C++ that will result in 
     * growth of java.lang.ref.Finalizer
     */
    if (NULL!=jsvcinfo)
    {
        (*NDRXJ_JENV(ctxpriv))->DeleteLocalRef(NDRXJ_JENV(ctxpriv), jsvcinfo);
    }
    
    if (NULL!=jcltid)
    {
        (*NDRXJ_JENV(ctxpriv))->DeleteLocalRef(NDRXJ_JENV(ctxpriv), jcltid);
    }
    
    if (NULL!=jdata)
    {
        (*NDRXJ_JENV(ctxpriv))->DeleteLocalRef(NDRXJ_JENV(ctxpriv), jdata);
    }
    
    if (NULL!=jname)
    {
        (*NDRXJ_JENV(ctxpriv))->DeleteLocalRef(NDRXJ_JENV(ctxpriv), jname);
    }
    
    if (NULL!=jfname)
    {
        (*NDRXJ_JENV(ctxpriv))->DeleteLocalRef(NDRXJ_JENV(ctxpriv), jfname);
    }
    
    NDRX_LOG(log_info, "%s return", __func__);
    
}

/*
 * Class:     org_endurox_AtmiCtx
 * Method:    tpAdvertiseC
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
expublic void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpadvertiseC
      (JNIEnv *env, jobject obj, jstring svcname, jstring funcname)
{
    
    /* What about context? */
    
    if (NULL== ndrxj_get_ctx(env, obj, EXTRUE))
    {
        goto out;
    }
    
    /* Hmm we could do advertise directly here the hash table could be stored
     * at C level, no need to proxy up to java for switching the service 
     * only then we might have some issues with garbage collector. So better
     * may be still do that in java side
     */
    jboolean n_svcname_copy = EXFALSE;
    const char *n_svcname = (*env)->GetStringUTFChars(env, svcname, &n_svcname_copy);
    
    jboolean n_funcname_copy = EXFALSE;
    const char *n_funcname = (*env)->GetStringUTFChars(env, funcname, &n_funcname_copy);
    
    if (EXSUCCEED!=tpadvertise_full((char *)n_svcname, dispatch_call, (char *)n_funcname))
    {
        NDRX_LOG(log_error, "Failed to advertise service [%s] func [%s]: %s",
                n_svcname, n_funcname, tpstrerror(tperrno));
        
        ndrxj_atmi_throw(env, NULL, NULL, tperrno, tpstrerror(tperrno));
        goto out;
    }
    
out:
    if (n_svcname_copy)
    {
        (*env)->ReleaseStringUTFChars(env, svcname, n_svcname);
    }

    if (n_funcname_copy)
    {
        (*env)->ReleaseStringUTFChars(env, funcname, n_funcname);
    }

    tpsetctxt(TPNULLCONTEXT, 0L);

}


/**
 * Shutdown of the server process
 */
exprivate void ndrxj_tpsvrdone(void)
{
    int ret = EXSUCCEED;
    jobject svrObj;
    ndrx_ctx_priv_t *ctxpriv;
    
    ctxpriv = ndrx_ctx_priv_get();
    
    svrObj = (*NDRXJ_JENV(ctxpriv))->GetObjectField(NDRXJ_JENV(ctxpriv), 
            NDRXJ_JATMICTX(ctxpriv), ndrxj_clazz_AtmiCtx_fid_svr);
    
    if (NULL==svrObj)
    {
        NDRX_LOG(log_error, "%s: Failed to get server interface object value!",
                __func__);
        EXFAIL_OUT(ret);
    }
    
    /* Now invoke the interface method */
 
    NDRX_LOG(log_debug, "About to call server interface (tpSvrDone)...");
    
    /* We shall enter into NULL context, not? */
    tpsetctxt(TPNULLCONTEXT, 0L);
    
    /* Call server object */
    (*NDRXJ_JENV(ctxpriv))->CallVoidMethod(NDRXJ_JENV(ctxpriv), svrObj, 
            ndrxj_clazz_Server_mid_tpSvrDone, NDRXJ_JATMICTX(ctxpriv));
    
    /* set back actual context */
    tpsetctxt(NDRXJ_CCTX(ctxpriv), 0L);
    
out:
            
    return;
}

/**
 * Run the server entry point...
 * TODO:  NewGlobalRef(JNIEnv *env, jobject obj); - use for context object?
 * @param env Java env
 * @param obj ATMI Context
 * @param jargv command line arguments passed to Java
 */
expublic jint JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpRunC(JNIEnv *env, jobject obj, 
        jobjectArray jargv, jboolean nocheck)
{
    char **argv = NULL;
    int argc = 0;
    int ret = EXSUCCEED;
    int size;
    int i;
    jstring jstr;
    jboolean n_elm_copy = EXFALSE;
    const char *n_elm;
    ndrx_ctx_priv_t *ctxpriv;
    TPCONTEXT_T ctx;
    
    
    if (NULL== (ctx=ndrxj_get_ctx(env, obj, EXTRUE)))
    {
        ret=EXFAIL;
        goto out;
    }
    
    /* WELL WE SHALL GET CONTEXT HERE!!!! The one allocated by java*/
    ctxpriv = ndrx_ctx_priv_get();
    
    M_jargv = jargv;
        
    if (NULL!=jargv)
    {
        size = (int)(*env)->GetArrayLength(env, jargv);
    }
    else
    {
        size = 0;
    }

    /* lock up th context object */
    obj=(*env)->NewGlobalRef(env, obj);

    NDRXJ_JENV_LVAL(ctxpriv) = env;
    
    /* shall we lock the global */
    
    NDRXJ_JATMICTX_LVAL(ctxpriv) = obj;
    NDRXJ_CCTX_LVAL(ctxpriv) = ctx;
    /* Mark us as a server context */
    NDRXJ_CTXFLAGS(ctxpriv)|=NDRXJ_CTXFLAGS_SRV;

    if (!nocheck)
    {
        if (size < 4)
        {
            ndrxj_atmi_throw(env, NULL, NULL, TPEINVAL, "Invalid argument count for server, "
                    "expected at least 4, got %d", size);
            EXFAIL_OUT(ret);
        }
    }
    
    argv = NDRX_CALLOC(sizeof(char *), size+1);
    
    if (NULL==argv)
    {
        int err = errno;
        NDRX_LOG(log_error, "Failed to realloc %d bytes: %s",   
            size, strerror(err));
        ndrxj_atmi_throw(env, NULL, NULL, TPESYSTEM, "Failed to realloc %d bytes: %s",
            size);
        EXFAIL_OUT(ret);
    }
    
    /* we shall get the process name as the first argument */
    /* we shall call Enduro/X libs here to get the program name */
    argv[0] = NDRX_STRDUP(EX_PROGNAME);

    /* loop over the argument */
    for (i=0; i<size; i++)
    {
        jstr = (jstring)(*env)->GetObjectArrayElement(env, jargv, i);
        
        if (NULL==jstr)
        {
            NDRX_LOG(log_error, "Failed to argv argument [%d]", i);
            EXFAIL_OUT(ret);
        }
        
        n_elm = (*env)->GetStringUTFChars(env, jstr, &n_elm_copy);
        
        if (NULL==(argv[i+1] = NDRX_STRDUP(n_elm)))
        {
            int err = errno;
            NDRX_LOG(log_error, "Failed to strdup bytes: %s",   
                strerror(err));
            ndrxj_atmi_throw(env, NULL, NULL, TPESYSTEM, "Failed to strdup bytes: %s",
                strerror(err));
            
            if (n_elm_copy)
            {
                (*env)->ReleaseStringUTFChars(env, jstr, n_elm);
            }
            
            EXFAIL_OUT(ret);
        }
        
        if (n_elm_copy)
        {
            (*env)->ReleaseStringUTFChars(env, jstr, n_elm);
        }
    }

    for (i=0; i<size+1; i++)
    {
        NDRX_LOG(log_debug, "argv[%d] = [%s]", i, argv[i]);
    }
    
    NDRX_LOG(log_info, "Booting java server..");
    
    argc=size+1;
    ret=ndrx_main_integra(argc, argv, ndrxj_tpsvrinit,
        ndrxj_tpsvrdone, ATMI_SRVLIB_NOLONGJUMP);
    
    /* Throw exception if any... */
    if (EXSUCCEED!=ret && 0!=tperrno)
    {
        ndrxj_atmi_throw(env, NULL, NULL, tperrno, tpstrerror(tperrno));
    }
    
out:

    /* go to NULL context */
    tpsetctxt(TPNULLCONTEXT, 0L);

    if (NULL!=argv)
    {
        for (i=0; i<size+1; i++)
        {
            if (NULL!=argv[i])
            {
                NDRX_FREE(argv[i]);
            }
        }

        NDRX_FREE(argv);
    }

    (*env)->DeleteGlobalRef(env, obj);

    return (jint)ret;
}

/**
 * Perform tpreturn
 * @param env java env
 * @param obj Context object
 * @param rval return value
 * @param rcode return code (user)
 * @param data data buffer
 * @param flags return flags
 */
expublic JNIEXPORT void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpreturn
  (JNIEnv *env, jobject atmiCtxObj, jint rval, jlong rcode, jobject data, jlong flags)
{
    int ret = EXSUCCEED;
    /* set context */
    char *buf = NULL;
    long len = 0;
    
    /* set ctx from obj */
    if (NULL == ndrxj_get_ctx(env, atmiCtxObj, EXTRUE))
    {
        return;
    }
    
    /* get data buffer... */
    if (NULL!=data)
    {
        if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &buf, &len, 
                NULL, EXTRUE, EXTRUE))
        {
            NDRX_LOG(log_error, "Failed to get data buffer!");
            EXFAIL_OUT(ret);
        }
    }
    
    tpreturn((int)rval, (long)rcode, buf, len, (long)flags);
 
out:

    NDRX_LOG(log_debug, "%s returns %d", __func__, ret);
    /* unset context */
    tpsetctxt(TPNULLCONTEXT, 0L);
}

/**
 * Backend for tpforward call
 * @param env java env
 * @param obj ATMI Context object
 * @param svcname service name
 * @param data data buffer
 * @param flags RFU flags
 */
expublic  JNIEXPORT void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpforward
  (JNIEnv *env, jobject atmiCtxObj, jstring svcname, jobject data, jlong flags)
{
    int ret = EXSUCCEED;
    /* set context */
    char *buf = NULL;
    long len = 0;
    jboolean n_svcname_copy = EXFALSE;
    const char *n_svcname = (*env)->GetStringUTFChars(env, svcname, &n_svcname_copy);
    
    /* set ctx from obj */
    if (NULL == ndrxj_get_ctx(env, atmiCtxObj, EXTRUE))
    {
        return;
    }
    
    /* get data buffer... */
    if (NULL!=data)
    {
        if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &buf, &len, 
                NULL, EXTRUE, EXTRUE))
        {
            NDRX_LOG(log_error, "Failed to get data buffer!");
            EXFAIL_OUT(ret);
        }
    }
    
    tpforward((char *)n_svcname, buf, len, (long)flags);
 
out:

    if (n_svcname_copy)
    {
        (*env)->ReleaseStringUTFChars(env, svcname, n_svcname);
    }

    NDRX_LOG(log_debug, "returns %d", ret);
    /* unset context */
    tpsetctxt(TPNULLCONTEXT, 0L);
}

/**
 * Terminate C context (tpterm + tpfreectxt)
 * @param env java env
 * @param cls static class
 * @param ctx C Context pointer
 */
expublic JNIEXPORT void JNICALL ndrxj_Java_org_endurox_AtmiCtx_finalizeC
  (JNIEnv *env, jclass cls, jlong cPtr)
{
    int freeWeak = EXFALSE;
    TPCONTEXT_T ctx = (TPCONTEXT_T)cPtr;
    /* TPCONTEXT_T tmp; */
    ndrx_ctx_priv_t *ctxpriv;
    jobject jctx;
    
    
    tpsetctxt(ctx, 0L);
    
    ctxpriv = ndrx_ctx_priv_get();
    jctx = NDRXJ_JATMICTX(ctxpriv);
    /* delete any weakref if have in context... */
    
    
    NDRX_LOG(log_debug, "About to free ctx %p", ctx);
    
    
    if (!(NDRXJ_CTXFLAGS(ctxpriv) & NDRXJ_CTXFLAGS_SRV) &&
	NULL!=NDRXJ_JATMICTX_LVAL(ctxpriv))
    {
        NDRX_LOG(log_debug, "Free up weakref %p", NDRXJ_JATMICTX_LVAL(ctxpriv));
        freeWeak = EXTRUE;
    }
    
    /*
    tpterm();
    tpsetctxt(TPNULLCONTEXT, 0L);
    */
    tpfreectxt(ctx);
    
    (*env)->DeleteWeakGlobalRef(env, jctx);
    
}

/**
 * Init the ATMI client
 * @param env java env
 * @param atmiCtxObj ATMI Context object
 * @param tpinfo TpInit object (infos)
 */
expublic JNIEXPORT void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpinit
  (JNIEnv * env, jobject atmiCtxObj, jobject tpinfo)
{
    TPCONTEXT_T ctx;
    
    if (NULL==(ctx = ndrxj_get_ctx(env, atmiCtxObj, EXTRUE)))
    {
        return;
    }
    
    if (EXSUCCEED!=tpinit(NULL))
    {
        ndrxj_atmi_throw(env, NULL, NULL, tperrno, tpstrerror(tperrno));
    }
    
    tpsetctxt(TPNULLCONTEXT, 0L);
    
}

/**
 * Write user log message
 * @param env Java ENV
 * @param atmiCtxObj ATMI Object
 * @param msg message to log
 */
expublic JNIEXPORT void JNICALL ndrxj_Java_org_endurox_AtmiCtx_userlogC
  (JNIEnv *env, jobject atmiCtxObj, jstring msg)
{
    jboolean n_msg_copy = EXFALSE;
    const char *n_msg;
    
    TPCONTEXT_T ctx;
    
    if (NULL==(ctx = ndrxj_get_ctx(env, atmiCtxObj, EXTRUE)))
    {
        return;
    }
    
    n_msg = (*env)->GetStringUTFChars(env, msg, &n_msg_copy);
    
    userlog("%s", n_msg);
    
    if (n_msg_copy)
    {
        (*env)->ReleaseStringUTFChars(env, msg, n_msg);
    }
    
    tpsetctxt(TPNULLCONTEXT, 0L);
}

/**
 * Compile Enduro/X UBF boolean expression
 * @param env Java env
 * @param ctx ATMI Context object
 * @param jexpr boolean expression string to compile
 * @return BExprTree object ptr
 */
expublic JNIEXPORT jobject JNICALL ndrxj_Java_org_endurox_AtmiCtx_Bboolco 
    (JNIEnv * env, jobject atmiCtxObj, jstring jexpr)
{
    jobject ret = NULL;
    jboolean n_expr_copy = EXFALSE;
    const char *n_expr;
    char *tree = NULL;
    /* set context */    
    TPCONTEXT_T ctx;

    if (NULL==(ctx = ndrxj_get_ctx(env, atmiCtxObj, EXTRUE)))
    {
        return NULL;
    }

    n_expr = (*env)->GetStringUTFChars(env, jexpr, &n_expr_copy);

    /* compile it! */
    tree = Bboolco((char *)n_expr);
    
    if (NULL==tree)
    {
        UBF_LOG(log_error, "Failed to compile: %s", Bstrerror(Berror));
        ndrxj_ubf_throw(env, Berror, "Failed to compile: %s", 
                Bstrerror(Berror));
        goto out;
    }
    
    /* create object.. */
    if (NULL==(ret = ndrxj_BExprTree_new(env, tree)))
    {
        UBF_LOG(log_error, "Failed to compile expression - object NULL!");
        goto out;
    }

out:
    
    if (NULL==ret && NULL!=tree)
    {
        Btreefree(tree);
    }

    if (n_expr_copy)
    {
        (*env)->ReleaseStringUTFChars(env, jexpr, n_expr);
    }

    tpsetctxt(TPNULLCONTEXT, 0L);

    return ret;
}

/**
 * Control data for compiled boolean expression printing
 */
typedef struct Bboolpr_ctl Bboolpr_ctl_t;
struct Bboolpr_ctl
{
    JNIEnv * env;
    jobject atmiCtxObj;
    jobject outstream;
};

/**
 * Just add the data/concat data buffer to dataptr...
 * @param buffer token to print
 * @param datalen token len including EOS byte
 * @param dataptr1 output byte
 * @return EXSUCCEED/EXFAIL
 */
exprivate int bboolprcb_callback(char *buffer, long datalen, void *dataptr1)
{
    Bboolpr_ctl_t *ctl = (Bboolpr_ctl_t *)dataptr1;
    jbyteArray ba = NULL;
    int ret = EXSUCCEED;
    TPCONTEXT_T context;
    
    /* create java byte array from received buffer */
    ba = (*(ctl->env))->NewByteArray(ctl->env, (jsize)datalen);

    if (NULL==ba)
    {
        NDRXJ_LOG_EXCEPTION((ctl->env), log_error, NDRXJ_LOGEX_ULOG, 
                "Failed to create byte array with size: %d: %s", (int)datalen);
        EXFAIL_OUT(ret);
    }
    
    /* setup byte array */
    (*(ctl->env))->SetByteArrayRegion(ctl->env, ba, 0, datalen, 
                            (jbyte*)buffer);

    if((*(ctl->env))->ExceptionCheck(ctl->env))
    {
        NDRXJ_LOG_EXCEPTION((ctl->env), log_error, NDRXJ_LOGEX_ULOG, 
                "Failed to set data bytes: %s");
        EXFAIL_OUT(ret);
    }
    
    /* suspend ATMI context as java might perform some other actions
     * on given thread.
     */
    tpgetctxt(&context, 0L);
    
    /* Call server object */
    (*(ctl->env))->CallVoidMethod(ctl->env, ctl->outstream, 
        ndrxj_clazz_OutputStream_mid_write, ba);
    
    /* restore ATMI context */
    tpsetctxt(context, 0L);

out:
    
    return ret;
}

/**
 * Print boolean expression to output stream
 * @param env java env
 * @param atmiCtxObj ATMI Context object
 * @param compexpr compiled expression
 * @param outstream output stream object
 */
JNIEXPORT void JNICALL ndrxj_Java_org_endurox_AtmiCtx_Bboolpr
  (JNIEnv * env, jobject atmiCtxObj, jobject compexpr, jobject outstream)
{
    /* set context */    
    TPCONTEXT_T ctx;
    char *tree;
    Bboolpr_ctl_t ctl;

    if (NULL==(ctx = ndrxj_get_ctx(env, atmiCtxObj, EXTRUE)))
    {
        return;
    }
    
    /* extract tree: compexpr */
    if (NULL==(tree = ndrxj_BExprTree_ptr_get(env, compexpr)))
    {
        UBF_LOG(log_error, "Failed to get expression handler!");
        goto out;
    }
    
    ctl.env = env;
    ctl.atmiCtxObj = atmiCtxObj;
    ctl.outstream = outstream;
    
    
    Bboolprcb(tree, bboolprcb_callback, &ctl);
    
out:

    tpsetctxt(TPNULLCONTEXT, 0L);
}

/**
 * Register callback function
 * @param env java env
 * @param atmiCtxObj ATMI Context object
 * @param funcname funcname
 */
expublic void JNICALL ndrxj_Java_org_endurox_AtmiCtx_BboolsetcbfC
  (JNIEnv * env, jobject atmiCtxObj, jstring funcname)
{
    TPCONTEXT_T ctx;
    jboolean n_funcname_copy = EXFALSE;
    const char *n_funcname = (*env)->GetStringUTFChars(env, funcname, 
    &n_funcname_copy);

    if (NULL==(ctx = ndrxj_get_ctx(env, atmiCtxObj, EXTRUE)))
    {
        return;
    }
    
    /* Register callback */
    
    if (EXSUCCEED!=Bboolsetcbf((char *)n_funcname,  Bbool_callback_function))
    {
        UBF_LOG(log_error, "Failed to call Bboolsetcbf(): %s", Bstrerror(Berror));
        ndrxj_ubf_throw(env, Berror, "Failed to call Bboolsetcbf(): %s", 
                Bstrerror(Berror));
        goto out;
    }
    
out:

    if (n_funcname_copy)
    {
        (*env)->ReleaseStringUTFChars(env, funcname, n_funcname);
    }

    tpsetctxt(TPNULLCONTEXT, 0L);
}

/**
 * Terminate the Atmi Context
 * @param env
 * @param atmiCtxObj
 */
expublic NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpterm
        (JNIEnv * env,  jobject atmiCtxObj)
{
    if (NULL==ndrxj_get_ctx(env, atmiCtxObj, EXTRUE))
    {
        return;
    }
    
    tpterm();
    
    tpsetctxt(TPNULLCONTEXT, 0L);
}

/**
 * Open XA Connection
 * @param env java env
 * @param atmiCtxObj Atmi Context obj
 */
expublic NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpopen
        (JNIEnv * env,  jobject atmiCtxObj)
{
    if (NULL==ndrxj_get_ctx(env, atmiCtxObj, EXTRUE))
    {
        return;
    }
    
    if (EXSUCCEED!=tpopen())
    {
        ndrxj_atmi_throw(env, NULL, NULL, tperrno, tpstrerror(tperrno));
    }
    
    tpsetctxt(TPNULLCONTEXT, 0L);
}

/**
 * Close XA sub-system, jdbc connection
 * @param env Java env
 * @param atmiCtxObj ATMI Context obj
 */
expublic NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpclose
  (JNIEnv * env, jobject atmiCtxObj)
{
    if (NULL==ndrxj_get_ctx(env, atmiCtxObj, EXTRUE))
    {
        return;
    }
    
    if (EXSUCCEED!=tpclose())
    {
        ndrxj_atmi_throw(env, NULL, NULL, tperrno, tpstrerror(tperrno));
    }
    
    tpsetctxt(TPNULLCONTEXT, 0L);
}

/**
 * Begin transaction
 * @param env java env
 * @param atmiCtxObj Atmi context
 * @param timeout timeout in seconds for transaction
 * @param flags flags
 */
expublic NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpbegin
  (JNIEnv * env, jobject atmiCtxObj, jlong timeout, jlong flags)
{
    if (NULL==ndrxj_get_ctx(env, atmiCtxObj, EXTRUE))
    {
        return;
    }
    
    if (EXSUCCEED!=tpbegin((unsigned long)timeout, (long)flags))
    {
        ndrxj_atmi_throw(env, NULL, NULL, tperrno, tpstrerror(tperrno));
    }
    
    tpsetctxt(TPNULLCONTEXT, 0L);
}

/**
 * Commit transaction
 * @param env java env
 * @param atmiCtxObj atmi ctx
 * @param flags flags
 */
expublic void NDRX_JAVA_API ndrxj_Java_org_endurox_AtmiCtx_tpcommit
  (JNIEnv * env, jobject atmiCtxObj, jlong flags)
{
    int ret;

    if (NULL==ndrxj_get_ctx(env, atmiCtxObj, EXTRUE))
    {
        return;
    }
    
    if (EXSUCCEED!=(ret=tpcommit((long)flags)))
    {
        NDRX_LOG(log_debug, "tpcommit returns %d", ret);
        ndrxj_atmi_throw(env, NULL, NULL, tperrno, tpstrerror(tperrno));
    }
    
    tpsetctxt(TPNULLCONTEXT, 0L);
}

/**
 * Abort transaction
 * @param env java env
 * @param atmiCtxObj atmi ctx
 * @param flags flags
 */
expublic void NDRX_JAVA_API ndrxj_Java_org_endurox_AtmiCtx_tpabort
  (JNIEnv *env, jobject atmiCtxObj, jlong flags)
{
    if (NULL==ndrxj_get_ctx(env, atmiCtxObj, EXTRUE))
    {
        return;
    }
    
    if (EXSUCCEED!=tpabort((long)flags))
    {
        ndrxj_atmi_throw(env, NULL, NULL, tperrno, tpstrerror(tperrno));
    }
    
    tpsetctxt(TPNULLCONTEXT, 0L);
}

/**
 * Suspend transaction
 * @param env java env
 * @param atmiCtxObj Atmi Context
 * @param flags flags
 * @return transaction id
 */
expublic jobject NDRX_JAVA_API ndrxj_Java_org_endurox_AtmiCtx_tpsuspend
  (JNIEnv *env, jobject atmiCtxObj, jlong flags)
{
    TPTRANID_conv tid;
    jobject jtid = NULL;
    
    if (NULL==ndrxj_get_ctx(env, atmiCtxObj, EXTRUE))
    {
        return NULL;
    }
    
    if (EXSUCCEED!=tpsuspend(&tid.tid, (long)flags))
    {
        ndrxj_atmi_throw(env, NULL, NULL, tperrno, tpstrerror(tperrno));
        goto out;
    }
    
    /* convert TID to java */
    jtid = ndrxj_atmi_TPTRANID_translate2java(env, 
            atmiCtxObj, NULL, &tid);
    
    if (NULL==jtid)
    {
        NDRX_LOG(log_error, "Failed to translate C TID");
    }
    
out:
    tpsetctxt(TPNULLCONTEXT, 0L);

    return jtid;
}

/**
 * Resume transaction
 * @param env java env
 * @param atmiCtxObj atmi ctx
 * @param[in] jtid transaction id
 * @param flags flags
 */
expublic void NDRX_JAVA_API ndrxj_Java_org_endurox_AtmiCtx_tpresume
  (JNIEnv * env, jobject atmiCtxObj, jobject jtid, jlong flags)
{
    int ret = EXSUCCEED;
    TPTRANID_conv tid;
    
    if (NULL==ndrxj_get_ctx(env, atmiCtxObj, EXTRUE))
    {
        return;
    }
    
    /* convert TID to c */
    if (EXSUCCEED!=ndrxj_atmi_TPTRANID_translate2c(env, 
            atmiCtxObj, jtid, &tid))
    {
        NDRX_LOG(log_error, "Failed to restore TID!");
        EXFAIL_OUT(ret);
    }
     
    if (EXSUCCEED!=tpresume(&tid.tid, (long)flags))
    {
        ndrxj_atmi_throw(env, NULL, NULL, tperrno, tpstrerror(tperrno));
        goto out;
    }
    
out:
    tpsetctxt(TPNULLCONTEXT, 0L);

}

/**
 * Get transaction level.
 * Are we in tran?
 * @param env java env
 * @param atmiCtxObj atmi context
 * @return  0 - no tran started, 1 - we are in globla tran
 */
expublic jint NDRX_JAVA_API ndrxj_Java_org_endurox_AtmiCtx_tpgetlev(JNIEnv *env, 
        jobject atmiCtxObj)
{
    int ret = EXFAIL;
    
    if (NULL==ndrxj_get_ctx(env, atmiCtxObj, EXTRUE))
    {
        return ret;
    }
    
    ret=(jint)tpgetlev();
    
    tpsetctxt(TPNULLCONTEXT, 0L);
    
    return ret;
}

/**
 * Get server context data
 * @param env java env
 * @param atmiCtxObj ATMI Context
 * @return 
 */
expublic jlong NDRX_JAVA_API ndrxj_Java_org_endurox_AtmiCtx_tpsrvgetctxdata
  (JNIEnv * env, jobject atmiCtxObj)
{
    jlong ret = EXFAIL;
    
    if (NULL==ndrxj_get_ctx(env, atmiCtxObj, EXTRUE))
    {
        return ret;
    }
    
    ret=(jlong)tpsrvgetctxdata();
    
    if (0==ret)
    {
        ndrxj_atmi_throw(env, NULL, NULL, tperrno, tpstrerror(tperrno));
    }
    
    tpsetctxt(TPNULLCONTEXT, 0L);
    
    return ret;
}

/**
 * Set server call context data
 * @param env java env
 * @param atmiCtxObj ATMI Context
 * @param dataptr allocated server context data
 */
expublic void NDRX_JAVA_API ndrxj_Java_org_endurox_AtmiCtx_tpsrvsetctxdata
  (JNIEnv * env, jobject atmiCtxObj, jlong dataptr, jlong flags)
{
    if (NULL==ndrxj_get_ctx(env, atmiCtxObj, EXTRUE))
    {
        return;
    }
    
    if (EXSUCCEED!=tpsrvsetctxdata((char *)dataptr, (long)flags))
    {
        ndrxj_atmi_throw(env, NULL, NULL, tperrno, tpstrerror(tperrno));
    }
    
    tpsetctxt(TPNULLCONTEXT, 0L);
}

/**
 * Free server call context data
 * @param env
 * @param atmiCtxObj
 * @param dataptr
 */
expublic void NDRX_JAVA_API ndrxj_Java_org_endurox_AtmiCtx_tpsrvfreectxdata
  (JNIEnv * env, jobject atmiCtxObj, jlong dataptr)
{
    if (NULL==ndrxj_get_ctx(env, atmiCtxObj, EXTRUE))
    {
        return;
    }
    
    tpsrvfreectxdata((char *)dataptr);
    
    tpsetctxt(TPNULLCONTEXT, 0L);
}

/**
 * Continue the server work
 * @param env java env
 * @param atmiCtxObj atmi context
 */
expublic NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpcontinue
        (JNIEnv * env, jobject atmiCtxObj)
{
    if (NULL==ndrxj_get_ctx(env, atmiCtxObj, EXTRUE))
    {
        return;
    }
    
    tpcontinue();
    
    tpsetctxt(TPNULLCONTEXT, 0L);
}

/* vim: set ts=4 sw=4 et smartindent: */
