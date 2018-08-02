/**
 * @brief ATMI Context backing JNI functions
 *
 * @file exceptions.c
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
#include <stdlib.h>
#include <jni.h>
#include "org_endurox_AtmiCtx.h"
#include <atmi.h>
#include <oatmi.h>
#include <ndebug.h>
#include <nerror.h>
#include "libsrc.h"
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
#define ERROR_MAX               1024
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/


/* TODO: We need a functions to map the ATMI errors to exceptions, this could be table driven. */

/**
 * Throw ATMI error 
 * @param env java env
 * @param data Any typed buffer associated with error / tpcall result
 * @param err ATMI Errro code
 * @param msg message
 */
expublic void ndrxj_atmi_throw(JNIEnv *env, jobject data, int err, char *msgfmt, ...)
{
    char cls[256];
    char error[ERROR_MAX];
    jstring jerror;
    jclass ex;
    
    jobject exception = NULL;
    jmethodID mid;
    jfieldID data_fldid;
    
    va_list args;
    
    va_start (args, msgfmt);
    vsnprintf (error, sizeof(error), msgfmt, args);
    va_end (args);
    
    jerror = (*env)->NewStringUTF(env, error);
    
    snprintf(cls, sizeof(cls), "org/endurox/exceptions/Atmi%sException", 
            tpecodestr(err));
    
    NDRX_LOG(log_info, "Throwing: [%s]", cls);
    
    /*
    ex = (*env)->FindClass(env, cls);
    
    if (!ex)
    {
        NDRX_LOG(log_error, "exception  [%s] not found!!!!", cls);
        abort();
    }
        
     (*env)->ThrowNew(env, ex, error); */
    
    ex = (*env)->FindClass(env, cls);
    
    if (NULL==ex)
    {
        NDRX_LOG(log_error, "exception  [%s] not found!!!! - aborting!", cls);
        abort();
    }
    
    mid = (*env)->GetMethodID(env, ex, "<init>", "(Ljava/lang/String;)V");
    
    if (NULL==mid)
    {
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_NDRX, 
                "Cannot get constructor for ATMI exception: %s");
        return;
    }
    
    exception = (*env)->NewObject(env, ex, mid, jerror);
    
    if (NULL==exception)
    {
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_NDRX, 
                "Failed to create exception object: %s");
        return;
    }
    
    /* set data field if any */
    
    if (NULL!=data)
    {
        NDRX_LOG(log_debug, "Setting data object for exception");
        if (NULL==(data_fldid = (*env)->GetFieldID(env, ex, "data", 
                "Lorg/endurox/TypedBuffer;")))
        {
            NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_NDRX, 
                    "Failed to find data field in exception: %s");
            return;
        }
        
        /* set object */
        
        (*env)->SetObjectField(env, exception, data_fldid, data);
    }
    
    /* throw finally */
    (*env)->Throw(env, (jthrowable)exception);
    
}

/**
 * Throw Enduro/X standard library error 
 * @param env java env
 * @param err ATMI Errro code
 * @param msg message
 */
expublic void ndrxj_nstd_throw(JNIEnv *env, int err, char *msgfmt, ...)
{
    char cls[256];
    char error[ERROR_MAX];
    jclass ex;
    va_list args;
    va_start (args, msgfmt);
    vsnprintf (error, sizeof(error), msgfmt, args);
    va_end (args);
    
    snprintf(cls, sizeof(cls), "org/endurox/exceptions/Nstd%sException", ndrx_Necodestr(err));
    
    
    NDRX_LOG(log_info, "Throwing: [%s]", cls);
    
    ex = (*env)->FindClass(env, cls);
    
    if (!ex)
    {
        NDRX_LOG(log_error, "exception  [%s] not found!!!!", cls);
        abort();
    }
        
    (*env)->ThrowNew(env, ex, error);
}

/**
 * Throw UBF error
 * @param env java env
 * @param err ATMI Errro code
 * @param msg message
 */
expublic void ndrxj_ubf_throw(JNIEnv *env, int err, char *msgfmt, ...)
{
    char cls[256];
    char error[ERROR_MAX];
    jclass ex;
    va_list args;
    va_start (args, msgfmt);
    vsnprintf (error, sizeof(error), msgfmt, args);
    va_end (args);
    
    snprintf(cls, sizeof(cls), "org/endurox/exceptions/Ubf%sException", 
            Becodestr(err));
    
    NDRX_LOG(log_info, "Throwing: [%s]", cls);
    
    ex = (*env)->FindClass(env, cls);
    
    if (!ex)
    {
        NDRX_LOG(log_error, "exception  [%s] not found!!!!", cls);
        abort();
    }
        
    (*env)->ThrowNew(env, ex, msgfmt);
}

/**
 * Return stacktrace of the exception into allocated buffer
 * @param env Java env where exception is set
 * @return allocated string with exception data
 */
expublic char *ndrxj_exception_backtrace(JNIEnv *env)
{
    char *ret;
    jthrowable exc;
    jstring s;
    const char* utf;
    jboolean isCopy = EXFALSE;
    jmethodID toString = (*env)->GetMethodID(env, 
            (*env)->FindClass(env, "java/lang/Object"), 
            "toString", "()Ljava/lang/String;");

    exc = (*env)->ExceptionOccurred(env);

    s = (jstring)(*env)->CallObjectMethod(env, exc, toString);

    utf = (*env)->GetStringUTFChars(env, s, &isCopy);

    ret = NDRX_STRDUP(utf);
    
    if (isCopy)
    {
        (*env)->ReleaseStringUTFChars(env, s, utf);
    }
    
    return ret;
}

/* vim: set ts=4 sw=4 et cindent: */

