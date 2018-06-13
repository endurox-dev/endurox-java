/**
 * @brief ATMI Context backing JNI functions
 *
 * @file AtmiCtx.c
 */ 
/*
 * -----------------------------------------------------------------------------
 * Enduro/X Middleware Platform for Distributed Transaction Processing
 * Copyright (C) 2015-2018 Mavimax, Ltd. All Rights Reserved.
 * This software is released under one of the following licenses:
 * GPL or Mavimax's license for commercial use.
 * -----------------------------------------------------------------------------
 * GPL license:
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
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
#include <atmi.h>
#include <oatmi.h>
#include <ndebug.h>
#include <ondebug.h>
#include <oatmisrv_integra.h>
#include "libsrc.h"
#include <sys_unix.h>
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/

/* NOTE: we can have single main thread only, thus we may use globals */

/** Java env for server operations */
exprivate JNIEnv *M_srv_ctx_env = NULL;

/** Context object */
exprivate jobject M_srv_ctx_obj = NULL;

/** Resolve context */
exprivate TPCONTEXT_T M_srv_ctx = NULL;

/** Command line arguments as passed to the server runner */
exprivate jobjectArray M_jargv;

/*---------------------------Prototypes---------------------------------*/

/**
 * get context from current object
 * @param env java env
 * @param atmiCtxObj ATMI Context object
 * @return NULL (and expection set) or context data
 */
expublic TPCONTEXT_T ndrxj_get_ctx(JNIEnv *env, jobject atmiCtxObj, int do_set)
{
    TPCONTEXT_T ctx;
   
    jclass objClass = (*env)->GetObjectClass(env, atmiCtxObj);
    jfieldID myFieldID = (*env)->GetFieldID(env, objClass, "ctx", "J");
    jlong fieldVal = (*env)->GetLongField(env, atmiCtxObj, myFieldID);

    ctx = (TPCONTEXT_T)fieldVal;

    if (NULL==ctx)
    {
        ndrxj_atmi_throw(env, TPEINVAL, "NULL C context for ATMI Context OP!");
    }
    else
    {
        if (do_set)
        {
            tpsetctxt(ctx, 0L);
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
jint JNICALL Java_org_endurox_AtmiCtx_tpLogQInfo (JNIEnv *env, jobject obj, 
        jint lev, jlong flags)
{
    TPCONTEXT_T ctx;
    int ret = 0;
    
    if (NULL==(ctx = ndrxj_get_ctx(env, obj, EXFALSE)))
    {
        goto out;
    }
    
    ret = Otplogqinfo(&ctx, (int)lev, (long)flags);
    
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
void JNICALL Java_org_endurox_AtmiCtx_tpLogC(JNIEnv * env, jobject obj, jint lev, 
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
 * NDRX Log entry
 * @param env java nev
 * @param obj java object (Atmi Context)
 * @param lev log level
 * @param file log file or empty string if not detailed log user
 * @param line line number of -1 if no detailed log user
 * @param msg message to log
 */
void JNICALL Java_org_endurox_AtmiCtx_tpLogNdrxC(JNIEnv * env, jobject obj, jint lev, 
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
 * Free up the atmi context
 * @param env java env
 * @param cls class on which method is called (Atmi Context this case), static
 * @param cPtr pointer to context C object 
 */
void JNICALL Java_org_endurox_AtmiCtx_tpfreectxt(JNIEnv *env, 
        jclass cls, jlong cPtr)
{
    TPCONTEXT_T ctx = (TPCONTEXT_T)cPtr;
    
    tpfreectxt(ctx);
}

/*
 * Class:     org_endurox_AtmiCtx
 * Method:    tpAlloc
 * Signature: (Ljava/lang/String;Ljava/lang/String;J)Lorg/endurox/AtmiBuf;
 */
jobject JNICALL Java_org_endurox_AtmiCtx_tpAlloc (JNIEnv *env, jobject obj, 
        jstring btype, jstring bsubtype, jlong size)
{
    jobject ret = NULL;
    char *data;
    TPCONTEXT_T ctx;
    jboolean n_btype_copy = EXFALSE;
    const char *n_btype = (*env)->GetStringUTFChars(env, btype, &n_btype_copy);
    
    jboolean n_bsubtype_copy = EXFALSE;
    const char *n_bsubtype = (*env)->GetStringUTFChars(env, bsubtype, &n_bsubtype_copy);
    
    /* get context handler */

    /* exception will thown if invalid object... */
    if (NULL==(ctx = ndrxj_get_ctx(env, obj, EXFALSE)))
    {
        goto out;
    }

    /* set context */
    tpsetctxt(ctx, 0L);

    /* allocate buffer, if error throw exception  */
    data = tpalloc((char *)n_btype, (char *)n_bsubtype, (long)size);

    if (NULL==data)
    {
        int err = tperrno;
        /* Generate exception! */
        ndrxj_atmi_throw(env, err, tpstrerror(err));
        goto out;
    }
    
    /* Translate the handler to Java side */
    ret = ndrxj_atmi_AtmiBuf_translate(env,  obj, EXTRUE, data, size, 
            (char *)n_btype, (char *)n_bsubtype);
    

    
    
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
jlong JNICALL Java_org_endurox_AtmiCtx_tpnewctxt (JNIEnv *env, jclass cls)
{
    TPCONTEXT_T ctx = tpnewctxt(0, 1);

    if (NULL==ctx)
    {
        ndrxj_atmi_throw(env, TPESYSTEM, "Failed to allocate new ATMI context!");
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
JNIEXPORT jobject JNICALL Java_org_endurox_AtmiCtx_getAtmiError (JNIEnv *env, jobject obj)
{
    TPCONTEXT_T ctx;
    int err;
    jstring jstr;
    jobject errObj = NULL;
    
    /* exception will thrown if invalid object... */
    if (NULL==(ctx = ndrxj_get_ctx(env, obj, EXFALSE)))
    {
        return NULL;
    }

    tpsetctxt(ctx, 0L);

    NDRX_LOG(log_debug, "context: (%p)", ctx);
    err = tperrno;

    /* Get the class we wish to return an instance of */
    jclass errClazz = (*env)->FindClass(env, "org/endurox/ErrorTuple");

    /* Get the method id of an empty constructor in clazz */
    jmethodID constructor = (*env)->GetMethodID(env, errClazz, "<init>", "()V");

    /* Create an instance of clazz */
    errObj = (*env)->NewObject(env, errClazz, constructor);

    /* Get Field references */
    jfieldID param1Field = (*env)->GetFieldID(env, errClazz, "err", "I");
    jfieldID param2Field = (*env)->GetFieldID(env, errClazz, "msg", "Ljava/lang/String;");

    /* Set fields for object */
    (*env)->SetIntField(env, errObj, param1Field, err);

    jstr=(jstring)((*env)->NewStringUTF(env, tpstrerror(err)) );
    (*env)->SetObjectField(env, errObj,param2Field,(jobject)jstr);
    
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
    jclass objClass;
    jfieldID myFieldID;        
    jobject svrObj;
    
    /* Attribs of the server to invoke */
    jclass svrClass;
    jmethodID svr_mid;
    
    NDRX_LOG(log_info, "Into tpsrvinit -> java");
    objClass = (*M_srv_ctx_env)->GetObjectClass(M_srv_ctx_env, M_srv_ctx_obj);
    
    if (NULL==objClass)
    {
        NDRX_LOG(log_error, "%s: Failed to get object class for AtmiContext",
                __func__);
        EXFAIL_OUT(ret);   
    }
    
    myFieldID = (*M_srv_ctx_env)->GetFieldID(M_srv_ctx_env, objClass, "svr", 
            "Lorg/endurox/Server;");
    
    if (NULL==myFieldID)
    {
        NDRX_LOG(log_error, "%s: Failed to get svr => Lorg/endurox/Server of Context obj!",
                __func__);
        EXFAIL_OUT(ret);
    }
    
    svrObj = (*M_srv_ctx_env)->GetObjectField(M_srv_ctx_env, M_srv_ctx_obj, myFieldID);
    
    if (NULL==svrObj)
    {
        NDRX_LOG(log_error, "%s: Failed to get server interface object value!",
                __func__);
        EXFAIL_OUT(ret);
    }
    
    /* Now invoke the interface method */
    
    svrClass = (*M_srv_ctx_env)->GetObjectClass(M_srv_ctx_env, svrObj);
    
    if (NULL==svrClass)
    {
        NDRX_LOG(log_error, "%s: Failed to get server object class",
                __func__);
        EXFAIL_OUT(ret);
    }
    
    svr_mid = (*M_srv_ctx_env)->GetMethodID(M_srv_ctx_env, svrClass, "tpSvrInit",
            "(Lorg/endurox/AtmiCtx;[Ljava/lang/String;)I");
    
    if (NULL==svr_mid)
    {
        NDRX_LOG(log_error, "%s: Failed to get tpSvrInit() method!",
                __func__);
        EXFAIL_OUT(ret);
    }
 
    NDRX_LOG(log_debug, "About to call server interface...");
    
    /* We shall enter into NULL context, not? */
    tpsetctxt(TPNULLCONTEXT, 0L);
    
    /* Call server object */
    ret = (int)(*M_srv_ctx_env)->CallIntMethod(M_srv_ctx_env, svrObj, svr_mid,
            M_srv_ctx_obj, M_jargv);
    
    /* set back actual context */
    tpsetctxt(M_srv_ctx, 0L);
    
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
    jobject jsvcinfo;
    jclass bclz;
    jmethodID mid;
    
    if (NULL==(jsvcinfo = ndrxj_atmi_TpSvcInfo_translate(M_srv_ctx_env,
            M_srv_ctx_obj, EXTRUE, svcinfo)))
    {
        NDRX_LOG(log_error, "Failed to translate service call to java!");
    }
    else
    {
        NDRX_LOG(log_debug, "%s: Got java service info invoke service: [%s]", 
                __func__, svcinfo->name);
        
        /* Call method from Atmi Context, before that unset the C
         * context, because we do not know in what threads java does works.
         */
        bclz = (*M_srv_ctx_env)->FindClass(M_srv_ctx_env, "org/endurox/AtmiCtx");
    
        if (NULL==bclz)
        {        
            /* I guess we need to abort here! */
            NDRX_LOG(log_error, "Failed to find AtmiCtx - aborting...!");
            /* tpreturn fail or simulate time-out? or just abort?*/
            abort();
        }

        /* create buffer object... */
        mid = (*M_srv_ctx_env)->GetMethodID(M_srv_ctx_env, bclz, 
                "tpCallDispatch", "(Lorg/endurox/TpSvcInfo;)V");

        if (NULL==mid)
        {
            NDRX_LOG(log_error, "Cannot get call dispatcher method at C side!");
            abort();
        }
        
        
        /* unset context */
        tpsetctxt(TPNULLCONTEXT, 0L);
        
        (*M_srv_ctx_env)->CallVoidMethod(M_srv_ctx_env, M_srv_ctx_obj, 
                mid, jsvcinfo);
        
        /* Check exceptions, if have one, I guess we abort or return TPFAIL? 
         * If get get exception here, log down the output and abort the process
         * It is up to developer to handle the exceptions before doing
         * tpreturn...
         */
        if ((*M_srv_ctx_env)->ExceptionCheck(M_srv_ctx_env))
        {
            NDRXJ_LOG_EXCEPTION(M_srv_ctx_env, log_error, 
                NDRXJ_LOGEX_ULOG, "Service have thrown unexpected exception: "
                    "[%s] - ignoring (continue)");
            
            (*M_srv_ctx_env)->ExceptionClear(M_srv_ctx_env);
        }
        
        /* set context back... */
        tpsetctxt(M_srv_ctx, 0L);
    }
}

/*
 * Class:     org_endurox_AtmiCtx
 * Method:    tpAdvertiseC
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
expublic void JNICALL Java_org_endurox_AtmiCtx_tpAdvertiseC
      (JNIEnv *env, jobject obj, jstring svcname, jstring funcname)
{
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
        
        ndrxj_atmi_throw(env, tperrno, tpstrerror(tperrno));
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
}


/**
 * Shutdown of the server process
 */
exprivate void ndrxj_tpsvrdone(void)
{
    int ret = EXSUCCEED;
    jclass objClass;
    jfieldID myFieldID;        
    jobject svrObj;
    
    /* Attribs of the server to invoke */
    jclass svrClass;
    jmethodID svr_mid;
    
    objClass = (*M_srv_ctx_env)->GetObjectClass(M_srv_ctx_env, M_srv_ctx_obj);
    
    if (NULL==objClass)
    {
        NDRX_LOG(log_error, "%s: Failed to get object class for AtmiContext",
                __func__);
        EXFAIL_OUT(ret);   
    }
    
    myFieldID = (*M_srv_ctx_env)->GetFieldID(M_srv_ctx_env, objClass, "svr", 
            "Lorg/endurox/Server;");
    
    if (NULL==myFieldID)
    {
        NDRX_LOG(log_error, "%s: Failed to get svr => Lorg/endurox/Server of Context obj!",
                __func__);
        EXFAIL_OUT(ret);
    }
    
    svrObj = (*M_srv_ctx_env)->GetObjectField(M_srv_ctx_env, M_srv_ctx_obj, myFieldID);
    
    if (NULL==svrObj)
    {
        NDRX_LOG(log_error, "%s: Failed to get server interface object value!",
                __func__);
        EXFAIL_OUT(ret);
    }
    
    /* Now invoke the interface method */
    
    svrClass = (*M_srv_ctx_env)->GetObjectClass(M_srv_ctx_env, svrObj);
    
    if (NULL==svrClass)
    {
        NDRX_LOG(log_error, "%s: Failed to get server object class",
                __func__);
        EXFAIL_OUT(ret);
    }
    
    svr_mid = (*M_srv_ctx_env)->GetMethodID(M_srv_ctx_env, svrClass, "tpSvrDone",
            "(Lorg/endurox/AtmiCtx;)V");
    
    if (NULL==svr_mid)
    {
        NDRX_LOG(log_error, "%s: Failed to get tpSvrDone() method!",
                __func__);
        EXFAIL_OUT(ret);
    }
 
    NDRX_LOG(log_debug, "About to call server interface (tpSvrDone)...");
    
    /* We shall enter into NULL context, not? */
    tpsetctxt(TPNULLCONTEXT, 0L);
    
    /* Call server object */
    (*M_srv_ctx_env)->CallVoidMethod(M_srv_ctx_env, svrObj, svr_mid, M_srv_ctx_obj);
    
    /* set back actual context */
    tpsetctxt(M_srv_ctx, 0L);
    
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
expublic jint JNICALL Java_org_endurox_AtmiCtx_tpRunC(JNIEnv *env, jobject obj, 
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
    /*TODO: Check the NULL? */

    M_srv_ctx_env = env;
    M_srv_ctx_obj = obj;

    if (!nocheck)
    {
        if (size < 4)
        {
            ndrxj_atmi_throw(env, TPEINVAL, "Invalid argument count for server, "
                    "expected at least 4, got %d", size);
            EXFAIL_OUT(ret);
        }
    }
    
    if (NULL==(M_srv_ctx = ndrxj_get_ctx(env, obj, EXTRUE)))
    {
        EXFAIL_OUT(ret);
    }
    
    argv = NDRX_CALLOC(sizeof(char *), size+1);
    
    if (NULL==argv)
    {
        int err = errno;
        NDRX_LOG(log_error, "Failed to realloc %d bytes: %s",   
            size, strerror(err));
        ndrxj_atmi_throw(env, TPESYSTEM, "Failed to realloc %d bytes: %s",
            size, strerror(err));
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
            ndrxj_atmi_throw(env, TPESYSTEM, "Failed to strdup bytes: %s",
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
        ndrxj_atmi_throw(env, tperrno, tpstrerror(tperrno));
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
expublic JNIEXPORT void JNICALL Java_org_endurox_AtmiCtx_tpReturn
  (JNIEnv *env, jobject obj, jint rval, jlong rcode, jobject data, jlong flags)
{
    int ret = EXSUCCEED;
    /* set context */
    char *buf = NULL;
    long len = 0;
    
    tpsetctxt(M_srv_ctx, 0L);
    
    /* get data buffer... */
    if (NULL!=data)
    {
        if (EXSUCCEED!=ndrxj_atmi_AtmiBuf_get_buffer(env, data, &buf, &len))
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
expublic  JNIEXPORT void JNICALL Java_org_endurox_AtmiCtx_tpForward
  (JNIEnv *env, jobject obj, jstring svcname, jobject data, jlong flags)
{
    int ret = EXSUCCEED;
    /* set context */
    char *buf = NULL;
    long len = 0;
    jboolean n_svcname_copy = EXFALSE;
    const char *n_svcname = (*env)->GetStringUTFChars(env, svcname, &n_svcname_copy);

    tpsetctxt(M_srv_ctx, 0L);
    
    /* get data buffer... */
    if (NULL!=data)
    {
        if (EXSUCCEED!=ndrxj_atmi_AtmiBuf_get_buffer(env, data, &buf, &len))
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

    NDRX_LOG(log_debug, "%s returns %d", __func__, ret);
    /* unset context */
    tpsetctxt(TPNULLCONTEXT, 0L);
}

/**
 * Terminate C context (tpterm + tpfreectxt)
 * @param env java env
 * @param cls static class
 * @param ctx C Context pointer
 */
expublic JNIEXPORT void JNICALL Java_org_endurox_AtmiCtx_finalizeC
  (JNIEnv *env, jclass cls, jlong cPtr)
{
    TPCONTEXT_T ctx = (TPCONTEXT_T)cPtr;
    
    Otpterm(&ctx);
    
    tpfreectxt(ctx);
    
}

/* vim: set ts=4 sw=4 et cindent: */
