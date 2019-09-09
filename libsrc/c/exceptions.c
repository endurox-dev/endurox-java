/**
 * @brief ATMI Context backing JNI functions
 *
 * @file exceptions.c
 */
/* -----------------------------------------------------------------------------
 * Enduro/X Middleware Platform for Distributed Transaction Processing
 * Copyright (C) 2009-2016, ATR Baltic, Ltd. All Rights Reserved.
 * Copyright (C) 2017-2019, Mavimax, Ltd. All Rights Reserved.
 * This software is released under one of the following licenses:
 * LGPL or Mavimax's license for commercial use.
 * See LICENSE text.
 *
 * C (as designed by Dennis Ritchie and later authors) language code is licensed
 * under Enduro/X Modified GNU Affero General Public License, version 3.
 * See LICENSE_C text.
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
#include <stdlib.h>
#include <jni.h>
#include <exjglue.h>
#include <atmi.h>
#include <oatmi.h>
#include <ndebug.h>
#include <nerror.h>
#include <exstring.h>
#include "libsrc.h"
#include <exjldsys.h>
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
 * @param[in] addarg1 additional argument 1, error specific
 * @param err ATMI Errro code
 * @param msg message
 */
expublic void ndrxj_atmi_throw(JNIEnv *env, jobject data, jobject addarg1, 
        int err, char *msgfmt, ...)
{
    char cls[256];
    char error[ERROR_MAX];
    jstring jerror;
    
    jobject exception = NULL;
    /* jfieldID data_fldid; */
    
    exj_dyn_cache_t new_cache;
    exj_dyn_cache_t *cached;
    
    va_list args;
    
    va_start (args, msgfmt);
    vsnprintf (error, sizeof(error), msgfmt, args);
    va_end (args);
    
    jerror = (*env)->NewStringUTF(env, error);
    
    snprintf(cls, sizeof(cls), "org/endurox/exceptions/Atmi%sException", 
            tpecodestr(err));
    
    NDRX_LOG(log_info, "Throwing: [%s]: %s", cls, error);
    
    cached = ndrxj_caches_get(cls);
    
    if (NULL==cached)
    {
        memset(&new_cache, 0, sizeof(new_cache));
        
        new_cache.clazz = (*env)->FindClass(env, cls);

        if (NULL==new_cache.clazz)
        {
            NDRX_LOG(log_error, "exception  [%s] not found!!!! - aborting!", cls);
            abort();
        }
        
        new_cache.mid1 = (*env)->GetMethodID(env, new_cache.clazz,
                "<init>", "(Ljava/lang/String;)V");

        if (NULL==new_cache.mid1)
        {
            NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_NDRX, 
                    "Cannot get constructor for ATMI exception: %s");
            return;
        }
        
        if (TPEDIAGNOSTIC == err)
        {
            /* In this case addarg1 qctl buffer associated with the queue
             * call
             */
            
            NDRX_LOG(log_debug, "Getting qctl for TPEDIAGNOSTIC");

            if (NULL==(new_cache.fid1 = (*env)->GetFieldID(env, new_cache.clazz, 
                    "qctl", "Lorg/endurox/TPQCTL;")))
            {
                NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_NDRX, 
                        "Failed to find data field in exception: %s");
            }
        }
        
        if (NULL==(new_cache.fid2 = (*env)->GetFieldID(env, new_cache.clazz, "data", 
                "Lorg/endurox/TypedBuffer;")))
        {
            NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_NDRX, 
                    "Failed to find data field in exception: %s");
            return;
        }
        
        if (NULL==(cached = ndrxj_caches_add(env, cls, &new_cache)))
        {
            NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_NDRX | NDRXJ_LOGEX_ULOG, 
                        "Failed to cached exception class!");
            abort();
        }
        
    }
    
    exception = (*env)->NewObject(env, cached->clazz, cached->mid1, jerror);
    
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
        
        
        /* set object */
        
        (*env)->SetObjectField(env, exception, cached->fid2, data);
    }
    
    if (NULL!=addarg1 && TPEDIAGNOSTIC == err)
    {
        /* set object */
        (*env)->SetObjectField(env, exception, cached->fid1, addarg1);
    }
    
    /* throw finally */
    (*env)->Throw(env, (jthrowable)exception);
    
out:
    
    return;
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
    exj_dyn_cache_t *cached;
    
    va_list args;
    va_start (args, msgfmt);
    vsnprintf (error, sizeof(error), msgfmt, args);
    va_end (args);
    
    snprintf(cls, sizeof(cls), "org/endurox/exceptions/Nstd%sException", 
            ndrx_Necodestr(err));
    
    
    NDRX_LOG(log_info, "Throwing: [%s]: %s", cls, error);
    
    cached = ndrxj_caches_single(env, cls);
    
    if (!cached)
    {
        NDRX_LOG(log_error, "exception  [%s] not found!!!!", cls);
        abort();
    }
        
    (*env)->ThrowNew(env, cached->clazz, error);
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
    exj_dyn_cache_t *cached;
    
    va_list args;
    va_start (args, msgfmt);
    vsnprintf (error, sizeof(error), msgfmt, args);
    va_end (args);
    
    snprintf(cls, sizeof(cls), "org/endurox/exceptions/Ubf%sException", 
            Becodestr(err));
    
    NDRX_LOG(log_info, "Throwing: [%s]: %s", cls, error);
    
    cached = ndrxj_caches_single(env, cls);
    
    if (!cached)
    {
        NDRX_LOG(log_error, "exception  [%s] not found!!!!", cls);
        abort();
    }
        
    (*env)->ThrowNew(env, cached->clazz, error);
}

/**
 * Backtrace exception frames
 * @param env java env
 * @param exc exception object
 * @param s string to build
 * @param mid_throwable_getCause
 * @param mid_throwable_getStackTrace
 * @param mid_throwable_toString
 * @param mid_frame_toString
 */
exprivate void backtrace_recursive(
                        JNIEnv*      env,
                        jthrowable   exc,
                        EX_string *s,
                        jmethodID    mid_throwable_getCause,
                        jmethodID    mid_throwable_getStackTrace,
                        jmethodID    mid_throwable_toString,
                        jmethodID    mid_frame_toString)
{
    /* get the stack trace */
    jobjectArray frames =
        (jobjectArray) (*env)->CallObjectMethod(env,
                                        exc,
                                        mid_throwable_getStackTrace);
    
    /* size of frames */
    jsize frames_length = 0;
    
    if (NULL!=frames)
    {
        frames_length = (*env)->GetArrayLength(env, frames);
    }

    if (0 != frames)
    {
        jboolean m_msg_obj_copy = EXFALSE;
        jstring msg_obj =
            (jstring) (*env)->CallObjectMethod(env, exc,
                               mid_throwable_toString);
        
        if (NULL!=msg_obj)
        {
            const char* msg_str = (*env)->GetStringUTFChars(env, msg_obj, 
                                &m_msg_obj_copy);

            exstring_strcat(s, msg_str);
            exstring_strcat(s, "\n");

            if (m_msg_obj_copy)
            {
                (*env)->ReleaseStringUTFChars(env, msg_obj, msg_str);
            }
        
            (*env)->DeleteLocalRef(env, msg_obj);
        }
    }

    if (frames_length > 0)
    {
        jsize i = 0;
        for (i = 0; i < frames_length; i++)
        {
            jobject frame = (*env)->GetObjectArrayElement(env, frames, i);
            
            /* when we have out of memory, we face problems here.. */
            if (NULL==frame)
            {
                continue;
            }
            
            jstring msg_obj =
                (jstring) (*env)->CallObjectMethod(env, frame,
                                                     mid_frame_toString);

            jboolean m_msg_obj_copy = EXFALSE;
            
            if (NULL!=msg_obj)
            {
                const char* msg_str = (*env)->GetStringUTFChars(env, msg_obj, 
                    &m_msg_obj_copy);
            
                exstring_strcat(s, msg_str);
                exstring_strcat(s, "\n");
            
                if (m_msg_obj_copy)
                {
                    (*env)->ReleaseStringUTFChars(env, msg_obj, msg_str);
                }
            
                (*env)->DeleteLocalRef(env, msg_obj);
            }
            (*env)->DeleteLocalRef(env, frame);
        }
    }

    /* print any inner exceptions */
    if (0 != frames)
    {
        jthrowable cause = 
            (jthrowable) (*env)->CallObjectMethod(env,
                            exc,
                            mid_throwable_getCause);
        if (0 != cause)
        {
            backtrace_recursive(env,
                                             cause,
                                             s,
                                             mid_throwable_getCause,
                                             mid_throwable_getStackTrace,
                                             mid_throwable_toString,
                                             mid_frame_toString);
        }
        
        if (NULL!=cause)
        {
            (*env)->DeleteLocalRef(env, cause);
        }
    }
    
    /* delete local ref */
    (*env)->DeleteLocalRef(env, frames);
}

/**
 * Return stacktrace of the exception into allocated buffer
 * @param env Java env where exception is set
 * @param exc_in   Input exception
 * @return allocated string with exception data
 */
expublic char *ndrxj_exception_backtrace(JNIEnv *env, jthrowable exc_in)
{
    char *ret = NULL;
    jthrowable exc = NULL;
    EX_string *ctrace = NULL;
    
    exstring_new(ctrace);

    if (NULL!=exc_in)
    {
        exc = exc_in;
    }
    else
    {
        exc = (*env)->ExceptionOccurred(env);
    }

    backtrace_recursive(
                        env,
                        exc,
                        ctrace,
                        ndrxj_clazz_Throwable_mid_getCause,
                        ndrxj_clazz_Throwable_mid_getStackTrace,
                        ndrxj_clazz_Throwable_mid_toString,
                        ndrxj_clazz_StackTraceElement_mid_toString);
    
    ret = NDRX_STRDUP(ctrace->d);
    
out:
    
    exstring_free(ctrace);

    if (NULL!=exc)
    {
        (*env)->DeleteLocalRef(env, exc);
    }

    /* userlog("Backtrace done: %s", ret); */
    
    return ret;
}

/* vim: set ts=4 sw=4 et smartindent: */
