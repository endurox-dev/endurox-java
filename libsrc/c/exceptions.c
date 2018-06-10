/* 
** Exceptions from JNI code
**
** @file exceptions.c
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
#include <stdlib.h>
#include <jni.h>
#include "org_endurox_AtmiCtx.h"
#include <atmi.h>
#include <oatmi.h>
#include <ndebug.h>
#include <nerror.h>
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
 * @param err ATMI Errro code
 * @param msg message
 */
expublic void ndrxj_atmi_throw(JNIEnv *env, int err, char *msgfmt, ...)
{
    char cls[256];
    char error[ERROR_MAX];
    jclass ex;
    va_list args;
    va_start (args, msgfmt);
    vsnprintf (error, sizeof(error), msgfmt, args);
    va_end (args);
    
    snprintf(cls, sizeof(cls), "org/endurox/exceptions/Atmi%sException", 
            tpecodestr(err));
    
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
    return NULL;
}

/* vim: set ts=4 sw=4 et cindent: */