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
expublic TPCONTEXT_T ndrxj_get_ctx(JNIEnv *env, jobject atmiCtxObj)
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
    
    if (NULL==(ctx = ndrxj_get_ctx(env, obj)))
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
    
    if (NULL==(ctx = ndrxj_get_ctx(env, obj)))
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
    
    if (NULL==(ctx = ndrxj_get_ctx(env, obj)))
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
    jclass bclz;
    TPCONTEXT_T ctx;
    jmethodID mid;
    char *buf;
    jboolean n_btype_copy = EXFALSE;
    jboolean n_bsubtype_copy = EXFALSE;
    
    const char *n_btype = (*env)->GetStringUTFChars(env, btype, &n_btype_copy);
    const char *n_bsubtype = (*env)->GetStringUTFChars(env, bsubtype, &n_bsubtype_copy);
    char clazz[256];
    /* get context handler */

    /* exception will thown if invalid object... */
    if (NULL==(ctx = ndrxj_get_ctx(env, obj)))
    {
        goto out;
    }

    /* set context */
    tpsetctxt(ctx, 0L);


    /* allocate buffer, if error throw exception  */
    buf = tpalloc((char *)n_btype, (char *)n_bsubtype, (long)size);

    if (NULL==buf)
    {
        int err = tperrno;
        /* Generate exception! */
        ndrxj_atmi_throw(env, err, tpstrerror(err));
        goto out;
    }
    
    /* allocate the object here according to the buffer type */
    
    if (0==strncmp(n_btype, "UBF", 3) || 
            0==strncmp(n_btype, "FML", 3))
    {
        /* UBF object */
        snprintf(clazz, sizeof(clazz), "org/endurox/TypedUbf");
    }
    else if (0==strcmp(n_btype, "CARRAY"))
    {
        /* Carray object */
        snprintf(clazz, sizeof(clazz), "org/endurox/TypedCarray");
    }
    else if (0==strcmp(n_btype, "STRING"))
    {
        /* String object */
        snprintf(clazz, sizeof(clazz), "org/endurox/TypedString");
    }
    else if (0==strcmp(n_btype, "VIEW"))
    {
        /* VIEW object */
        snprintf(clazz, sizeof(clazz), "org/endurox/TypedView");
    }
    else if (0==strcmp(n_btype, "JSON"))
    {
        /* JSON object */
        snprintf(clazz, sizeof(clazz), "org/endurox/TypedJson");
    }
    else
    {
        ndrxj_atmi_throw(env, TPEINVAL, "buffer type [%s] not supported", 
                n_btype);
        goto out;
    }
    
    
    NDRX_LOG(log_debug, "Allocating [%s] class", clazz);
    
    bclz = (*env)->FindClass(env, clazz);
    
    if (NULL==bclz)
    {        
        /* ndrxj_atmi_throw(env, TPESYSTEM, "Class not found [%s]", clazz); */
        NDRX_LOG(log_error, "Failed to find class [%s]", clazz);
        goto out;
        
    }
    
    /* create buffer object... */
    mid = (*env)->GetMethodID(env, bclz, "<init>", "(Lorg/endurox/AtmiCtx;ZJJ)V");
    
    if (NULL==mid)
    {
        NDRX_LOG(log_error, "Cannot get buffer constructor!");
        goto out;
    }

    NDRX_LOG(log_debug, "About to NewObject()");
    ret = (*env)->NewObject(env, bclz, mid, obj, JNI_TRUE, (jlong)buf, size);
    
    if (NULL==ret)
    {
        NDRX_LOG(log_error, "Failed to create [%s]", clazz);
        goto out;
    }
    
    NDRX_LOG(log_debug, "NewObject() done");
    
    /* unset context */
    tpsetctxt(TPNULLCONTEXT, 0L);
    

out:
    
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
    TPCONTEXT_T ctx = tpnewctxt(0, 0);

    if (NULL==ctx)
    {
        ndrxj_atmi_throw(env, TPESYSTEM, "Failed to allocate new ATMI context!");
    }
    else
    {
        NDRX_LOG(log_debug, "New ATMI context: %p", ctx);
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
    if (NULL==(ctx = ndrxj_get_ctx(env, obj)))
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
 * @param env Java env
 * @param obj ATMI Context
 * @param jargv command line arguments passed to Java
 */
jint JNICALL Java_org_endurox_AtmiCtx_TpRunC(JNIEnv *env, jobject obj, 
        jobjectArray jargv)
{
    M_srv_ctx_env = env;
    M_srv_ctx_obj = obj;
    char **argv = NULL;
    int argc = 0;
    int ret = EXSUCCEED;
    int size = (int)(*env)->GetArrayLength(env, jargv);
    int ctx_set = EXFALSE;
    int i;
    jstring jstr;
    jboolean n_elm_copy = EXFALSE;
    const char *n_elm;
    
    M_jargv = jargv;

    if (size < 4)
    {
        ndrxj_atmi_throw(env, TPEINVAL, "Invalid argument count for server, "
                "expected at least 4, got %d", size);
        EXFAIL_OUT(ret);
    }
    
    if (NULL==(M_srv_ctx = ndrxj_get_ctx(env, obj)))
    {
        EXFAIL_OUT(ret);
    }

    /* we have a context handler, we shall switch to it now... */
    tpsetctxt(M_srv_ctx, 0L);
    ctx_set=EXTRUE;
    
    argv = NDRX_CALLOC(sizeof(char *), size);
    
    if (NULL==argv)
    {
        int err = errno;
        NDRX_LOG(log_error, "Failed to realloc %d bytes: %s",   
            size, strerror(err));
        ndrxj_atmi_throw(env, TPESYSTEM, "Failed to realloc %d bytes: %s",
            size, strerror(err));
        EXFAIL_OUT(ret);
    }
    
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
        
        if (NULL==(argv[i] = NDRX_STRDUP(n_elm)))
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
    
    ret=Ondrx_main_integra(M_srv_ctx, argc, argv, ndrxj_tpsvrinit,
        ndrxj_tpsvrdone, 0L);
    
    /* Throw exception if any... */
    if (EXSUCCEED!=ret && 0!=tperrno)
    {
        ndrxj_atmi_throw(env, tperrno, tpstrerror(tperrno));
    }
    
out:

    /* go to NULL context */
    if (ctx_set)
    {
        tpsetctxt(TPNULLCONTEXT, 0L);
    }

    if (NULL!=argv)
    {
        for (i=0; i<size; i++)
        {
            if (NULL!=argv[i])
            {
                NDRX_FREE(argv[i]);
            }
        }

        NDRX_FREE(argv);
    }

    return (jint)ret;
}

/* vim: set ts=4 sw=4 et cindent: */
