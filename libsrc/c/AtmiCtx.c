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
#include "org_endurox_AtmiCtx.h"
#include <atmi.h>
#include <oatmi.h>
#include <ndebug.h>
#include <ondebug.h>
#include "libsrc.h"
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
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
    
    const char *n_file = (*env)->GetStringUTFChars(env, file, 0);
    const char *n_msg = (*env)->GetStringUTFChars(env, msg, 0);
    
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
    (*env)->ReleaseStringUTFChars(env, file, n_file);
    (*env)->ReleaseStringUTFChars(env, msg, n_msg);   
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
    
    const char *n_file = (*env)->GetStringUTFChars(env, file, 0);
    const char *n_msg = (*env)->GetStringUTFChars(env, msg, 0);
    
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
    (*env)->ReleaseStringUTFChars(env, file, n_file);
    (*env)->ReleaseStringUTFChars(env, msg, n_msg);   
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
    const char *n_btype = (*env)->GetStringUTFChars(env, btype, 0);
    const char *n_bsubtype = (*env)->GetStringUTFChars(env, bsubtype, 0);
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
    
    (*env)->ReleaseStringUTFChars(env, btype, n_btype);
    (*env)->ReleaseStringUTFChars(env, bsubtype, n_bsubtype);
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
    /* exception will thown if invalid object... */
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


