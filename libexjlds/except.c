/**
 * @brief Boot time exception handler
 *   this is copy from exceptions.c of libexjava
 *   not share because we do not make the dependency on this library
 *   but compiled unit only depends on Enduro/X Core.
 *
 * @file except.c
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
#include <stdio.h>
#include <errno.h>
#include <jni.h>
#include <ndebug.h>
#include <userlog.h>
#include <ndrstandard.h>
#include <inicfg.h>
#include <cconfig.h>
#include <exjlds.h>
#include <nstdutil.h>
#include <exstring.h>
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/


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
    jsize frames_length = (*env)->GetArrayLength(env, frames);

    if (0 != frames)
    {
        jboolean m_msg_obj_copy = EXFALSE;
        jstring msg_obj =
            (jstring) (*env)->CallObjectMethod(env, exc,
                               mid_throwable_toString);
        
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

    if (frames_length > 0)
    {
        jsize i = 0;
        for (i = 0; i < frames_length; i++)
        {
            jobject frame = (*env)->GetObjectArrayElement(env, frames, i);
            jstring msg_obj =
                (jstring) (*env)->CallObjectMethod(env, frame,
                                                     mid_frame_toString);

            jboolean m_msg_obj_copy = EXFALSE;
            
            const char* msg_str = (*env)->GetStringUTFChars(env, msg_obj, 
                    &m_msg_obj_copy);
            
            exstring_strcat(s, msg_str);
            exstring_strcat(s, "\n");
            
            if (m_msg_obj_copy)
            {
                (*env)->ReleaseStringUTFChars(env, msg_obj, msg_str);
            }
            
            (*env)->DeleteLocalRef(env, msg_obj);
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
expublic char *ndrxj_ldr_exception_backtrace(JNIEnv *env, jthrowable exc_in)
{
    char *ret = NULL;
    jthrowable exc = NULL;
    EX_string *ctrace = NULL;
    
    jmethodID mid_throwable_getCause;
    jmethodID mid_throwable_getStackTrace;
    jmethodID mid_throwable_toString;
    jmethodID mid_frame_toString;
    jclass frame_class = NULL;
    jclass throwable_class = NULL;
    
    throwable_class = (*env)->FindClass(env, "java/lang/Throwable");
    
    if (NULL==throwable_class)
    {
        userlog("Failed to get [java/lang/Throwable] class!");
        goto out;
    }
    
    mid_throwable_getCause =
        (*env)->GetMethodID(env, throwable_class, "getCause", 
            "()Ljava/lang/Throwable;");
    
    if (NULL==mid_throwable_getCause)
    {
        userlog("Failed to get [Throwable.getCause()] mid!");
        goto out;
    }

    mid_throwable_getStackTrace =
        (*env)->GetMethodID(env, throwable_class, "getStackTrace",
                          "()[Ljava/lang/StackTraceElement;");
    
    if (NULL==mid_throwable_getStackTrace)
    {
        userlog("Failed to get [Throwable.getStackTrace()] mid!");
        goto out;
    }
    
    mid_throwable_toString =
        (*env)->GetMethodID(env, throwable_class, "toString",
                          "()Ljava/lang/String;");
    
    if (NULL==mid_throwable_toString)
    {
        userlog("Failed to get [Throwable.toString()] mid!");
        goto out;
    }

    frame_class = (*env)->FindClass(env, "java/lang/StackTraceElement");
    
    if (NULL==frame_class)
    {
        userlog("Failed to find [java/lang/StackTraceElement] class!");
        goto out;
    }
    
    mid_frame_toString =
        (*env)->GetMethodID(env, frame_class,
                          "toString",
                          "()Ljava/lang/String;");
    if (NULL==mid_frame_toString)
    {
        userlog("Failed to find [StackTraceElement.toString()] mid!");
        goto out;
    }
    
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
                        mid_throwable_getCause,
                        mid_throwable_getStackTrace,
                        mid_throwable_toString,
                        mid_frame_toString);
    
    ret = NDRX_STRDUP(ctrace->d);
    
out:
    
    exstring_free(ctrace);

    if (NULL!=exc)
    {
        (*env)->DeleteLocalRef(env, exc);
    }

    if (NULL!=throwable_class)
    {
        (*env)->DeleteLocalRef(env, throwable_class);
    }

    if (NULL!=throwable_class)
    {
        (*env)->DeleteLocalRef(env, frame_class);
    }


    /* userlog("Backtrace done: %s", ret); */
    
    return ret;
}


/* vim: set ts=4 sw=4 et smartindent: */
