/**
 * @brief Logging (tplog) binding code
 *
 * @file logging.c
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

/*------------------------------Externs---------------------------------------*/
/*------------------------------Macros----------------------------------------*/
/*------------------------------Enums-----------------------------------------*/
/*------------------------------Typedefs--------------------------------------*/
/*------------------------------Globals---------------------------------------*/
/*------------------------------Statics---------------------------------------*/
/*------------------------------Prototypes------------------------------------*/

/**
 * Log exception to UBF logger
 * @param env java env
 * @param atmiCtxObj ATMI Context
 * @param lev log level
 * @param msg message to print
 * @param e exception to backtrace
 */
expublic JNIEXPORT void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tplogubfex
  (JNIEnv * env, jobject atmiCtxObj, jint lev, jstring msg, jthrowable e)
{
    
    jboolean n_msg_copy = EXFALSE;
    
    const char *n_msg = (*env)->GetStringUTFChars(env, msg, &n_msg_copy);

    
    if (NULL==ndrxj_get_ctx(env, atmiCtxObj, EXTRUE))
    {
        goto out;
    }
    
    NDRXJ_LOG_EXCEPTION_E(env, e, lev, NDRXJ_LOGEX_UBF, "%s: %s", n_msg);
    
out:
    tpsetctxt(TPNULLCONTEXT, 0L);

    
    if (n_msg_copy)
    {
        (*env)->ReleaseStringUTFChars(env, msg, n_msg);
    }

    return;
}

/**
 * log exception to NDRX logger
 * @param env java env
 * @param atmiCtxObj ATMI Context
 * @param lev debug level
 * @param msg custom message
 * @param e exception
 */
expublic JNIEXPORT void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tplogndrxex
  (JNIEnv * env, jobject atmiCtxObj, jint lev, jstring msg, jthrowable e)
{
    jboolean n_msg_copy = EXFALSE;
    
    const char *n_msg = (*env)->GetStringUTFChars(env, msg, &n_msg_copy);
 
    if (NULL==ndrxj_get_ctx(env, atmiCtxObj, EXTRUE))
    {
        goto out;
    }
    
    NDRXJ_LOG_EXCEPTION_E(env, e, lev, NDRXJ_LOGEX_NDRX, "%s: %s", n_msg);
    
out:
    tpsetctxt(TPNULLCONTEXT, 0L);

    
    if (n_msg_copy)
    {
        (*env)->ReleaseStringUTFChars(env, msg, n_msg);
    }

    return;
}

/**
 * log exception to TP logger
 * @param env java env
 * @param atmiCtxObj ATMI Context
 * @param lev debug level
 * @param msg custom message
 * @param e exception
 */
expublic JNIEXPORT void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tplogex
  (JNIEnv * env, jobject atmiCtxObj, jint lev, jstring msg, jthrowable e)
{
    jboolean n_msg_copy = EXFALSE;
    
    const char *n_msg = (*env)->GetStringUTFChars(env, msg, &n_msg_copy);
 
    if (NULL==ndrxj_get_ctx(env, atmiCtxObj, EXTRUE))
    {
        goto out;
    }
    
    NDRXJ_LOG_EXCEPTION_E(env, e, lev, NDRXJ_LOGEX_TP, "%s: %s", n_msg);
    
out:
    tpsetctxt(TPNULLCONTEXT, 0L);

    
    if (n_msg_copy)
    {
        (*env)->ReleaseStringUTFChars(env, msg, n_msg);
    }

    return;
}

/**
 * Write exception to "ULOG"
 * @param env java env
 * @param atmiCtxObj ATMI Context
 * @param lev debug level
 * @param msg custom message
 * @param e exception
 */
expublic JNIEXPORT void JNICALL ndrxj_Java_org_endurox_AtmiCtx_userlogex
  (JNIEnv * env, jobject atmiCtxObj, jstring msg, jthrowable e)
{
    jboolean n_msg_copy = EXFALSE;
    
    const char *n_msg = (*env)->GetStringUTFChars(env, msg, &n_msg_copy);
 
    if (NULL==ndrxj_get_ctx(env, atmiCtxObj, EXTRUE))
    {
        goto out;
    }
    
    NDRXJ_LOG_EXCEPTION_E(env, e, log_error, NDRXJ_LOGEX_ULOG, "%s: %s", n_msg);
    
out:
    tpsetctxt(TPNULLCONTEXT, 0L);

    
    if (n_msg_copy)
    {
        (*env)->ReleaseStringUTFChars(env, msg, n_msg);
    }

    return;
}

/**
 * Set request file
 * @param env java env
 * @param atmiCtxObj ATMI Context
 * @param data typed buffer obj
 * @param filename new file name (might be null)
 * @param filesvc service for file (might be null too)
 */
expublic NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tplogsetreqfile
        (JNIEnv *env, jobject atmiCtxObj, jobject data, jstring filename, jstring filesvc)
{
    char *buf = NULL;
    long len = 0;
    TPCONTEXT_T ctx;
    
    jboolean n_filename_copy = EXFALSE;
    const char *n_filename = NULL;
    
    jboolean n_filesvc_copy = EXFALSE;
    const char *n_filesvc = NULL;
    
    if (NULL==(ctx = ndrxj_get_ctx(env, atmiCtxObj, EXTRUE)))
    {
        goto out;
    }
    
    if (NULL!=data)
    {
        if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &buf, &len, 
                NULL, EXFALSE, EXFALSE))
        {
            NDRX_LOG(log_error, "Failed to get data buffer!");
            goto out;
        }
    }
    
    if (NULL!=filename)
    {
        n_filename = (*env)->GetStringUTFChars(env, filename, &n_filename_copy);
    }
    
    if (NULL!=filesvc)
    {
        n_filesvc = (*env)->GetStringUTFChars(env, filesvc, &n_filesvc_copy);
    }

    if (EXSUCCEED!=tplogsetreqfile(&buf, (char *)n_filename, (char *)n_filesvc))
    {
        ndrxj_atmi_throw(env, data, NULL, tperrno, "%s", tpstrerror(tperrno));
        goto out;
    }
    
    
out:
    
    if (n_filename_copy)
    {
        (*env)->ReleaseStringUTFChars(env, filename, n_filename);
    }

    if (n_filesvc_copy)
    {
        (*env)->ReleaseStringUTFChars(env, filesvc, n_filesvc);
    }

    tpsetctxt(TPNULLCONTEXT, 0L);

    return;
}

/**
 * Get request file from UBF buffer
 * @param env java env
 * @param atmiCtxObj ATMI Context
 * @param data typed buffer
 * @return request file name
 */
expublic NDRX_JAVA_API jstring JNICALL ndrxj_Java_org_endurox_AtmiCtx_tploggetbufreqfile
        (JNIEnv *env, jobject atmiCtxObj, jobject data)
{
    TPCONTEXT_T ctx;
    char *buf = NULL;
    long len = 0;
    char filename[PATH_MAX+1];
    jstring ret = NULL;
    
    if (NULL==(ctx = ndrxj_get_ctx(env, atmiCtxObj, EXTRUE)))
    {
        goto out;
    }
    
    if (NULL==data)
    {
        ndrxj_nullptr_throw(env, "`data' cannot be NULL");
        goto out;
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &buf, &len, 
                NULL, EXFALSE, EXFALSE))
    {
        NDRX_LOG(log_error, "Failed to get data buffer!");
        goto out;
    }
    
    if (EXSUCCEED!=tploggetbufreqfile(buf, filename, sizeof(filename)))
    {
        NDRX_LOG(log_error, "Failed to get req file from buffer!");
        ndrxj_atmi_throw(env, data, NULL, tperrno, "%s", tpstrerror(tperrno));
        goto out;
    }
    
     /* build java string... */
    ret = (*env)->NewStringUTF(env, filename);
    
out:
    
    tpsetctxt(TPNULLCONTEXT, 0L);

    return ret;
}

/**
 * Delete request file
 * @param env java env
 * @param atmiCtxObj ATMI context
 * @param data Typed buffer / UBF
 */
expublic NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tplogdelbufreqfile
        (JNIEnv *env, jobject atmiCtxObj, jobject data)
{
    TPCONTEXT_T ctx;
    char *buf = NULL;
    long len = 0;
    
    if (NULL==(ctx = ndrxj_get_ctx(env, atmiCtxObj, EXTRUE)))
    {
        goto out;
    }
    
    if (NULL==data)
    {
        ndrxj_nullptr_throw(env, "`data' cannot be NULL");
        goto out;
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &buf, &len, 
                NULL, EXFALSE, EXFALSE))
    {
        NDRX_LOG(log_error, "Failed to get data buffer!");
        goto out;
    }
    
    if (EXSUCCEED!=tplogdelbufreqfile(buf))
    {
        NDRX_LOG(log_error, "Failed to get req file from buffer!");
        ndrxj_atmi_throw(env, data, NULL, tperrno, "%s", tpstrerror(tperrno));
        goto out;
    }
    
out:
    
    tpsetctxt(TPNULLCONTEXT, 0L);

}

/**
 * Get real request file currently used by logger
 * @param env java env
 * @param atmiCtxObj ATMI Context
 * @return request filename used or NULL if not set
 */
expublic NDRX_JAVA_API jstring JNICALL ndrxj_Java_org_endurox_AtmiCtx_tploggetreqfile
        (JNIEnv *env, jobject atmiCtxObj)
{
    TPCONTEXT_T ctx;
    char filename[PATH_MAX+1];
    jstring ret = NULL;
    
    if (NULL==(ctx = ndrxj_get_ctx(env, atmiCtxObj, EXTRUE)))
    {
        goto out;
    }
    
    if (!tploggetreqfile(filename, sizeof(filename)))
    {
        NDRX_LOG(log_info, "Request file not set!");
        goto out;
    }
    
     /* build java string... */
    ret = (*env)->NewStringUTF(env, filename);
    
out:
    
    tpsetctxt(TPNULLCONTEXT, 0L);

    return ret;
}

/**
 * Set request file directly (no extra checking code)
 * @param env java env
 * @param atmiCtxObj ATMI Context
 * @param filename file name to set. If NULL, throws NullPointerException
 */
expublic NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tplogsetreqfile_1direct
        (JNIEnv *env, jobject atmiCtxObj, jstring filename)
{
    TPCONTEXT_T ctx;
    jboolean n_filename_copy = EXFALSE;
    const char *n_filename = NULL;
    
    if (NULL==(ctx = ndrxj_get_ctx(env, atmiCtxObj, EXTRUE)))
    {
        goto out;
    }
    
    if (NULL==filename)
    {
        ndrxj_nullptr_throw(env, "`filename' cannot be NULL");
        goto out;
    }
    
    n_filename = (*env)->GetStringUTFChars(env, filename, &n_filename_copy);
    
    
    tplogsetreqfile_direct((char *)n_filename);
    
out:
    
    if (n_filename_copy)
    {
        (*env)->ReleaseStringUTFChars(env, filename, n_filename);
    }

    tpsetctxt(TPNULLCONTEXT, 0L);

}

/**
 * Close request logging
 * @param env java env
 * @param atmiCtxObj ATMI Context
 */
expublic NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tplogclosereqfile
        (JNIEnv *env, jobject atmiCtxObj)
{
    TPCONTEXT_T ctx;
    
    if (NULL==(ctx = ndrxj_get_ctx(env, atmiCtxObj, EXTRUE)))
    {
        goto out;
    }
    
    tplogclosereqfile();
    
out:
    
    tpsetctxt(TPNULLCONTEXT, 0L);
}

/**
 * Configure logger
 * @param env java env
 * @param atmiCtxObj ATMI Context
 * @param logger logger faclity (bitmask)
 * @param lev debug level to set
 * @param debug_string debug string according to ndrxdebug.conf(5)
 * @param module module code
 * @param new_file new debug file name to use
 */
expublic NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tplogconfig
    (JNIEnv *env, jobject atmiCtxObj, jint logger, jint lev, jstring debug_string, 
        jstring module, jstring new_file)
{
    TPCONTEXT_T ctx;
    
    jboolean n_debug_string_copy = EXFALSE;
    const char *n_debug_string = NULL;
    
    jboolean n_module_copy = EXFALSE;
    const char *n_module = NULL;
    
    jboolean n_new_file_copy = EXFALSE;
    const char *n_new_file = NULL;
    
    if (NULL==(ctx = ndrxj_get_ctx(env, atmiCtxObj, EXTRUE)))
    {
        goto out;
    }
    
    if (NULL!=debug_string)
    {
        n_debug_string = (*env)->GetStringUTFChars(env, debug_string, &n_debug_string_copy);
    }
    
    if (NULL!=module)
    {
        n_module = (*env)->GetStringUTFChars(env, module, &n_module_copy);
    }
    
    if (NULL!=new_file)
    {
        n_new_file = (*env)->GetStringUTFChars(env, new_file, &n_new_file_copy);
    }
    
    if (EXSUCCEED!=tplogconfig((int)logger, (int)lev, (char *)n_debug_string, 
            (char *)n_module, (char *)n_new_file))
    {
        ndrxj_atmi_throw(env, NULL, NULL, tperrno, "%s", tpstrerror(tperrno));
        goto out;
    }
    
out:
    
    if (n_debug_string_copy)
    {
        (*env)->ReleaseStringUTFChars(env, debug_string, n_debug_string);
    }

    if (n_module_copy)
    {
        (*env)->ReleaseStringUTFChars(env, module, n_module);
    }

    if (n_new_file_copy)
    {
        (*env)->ReleaseStringUTFChars(env, new_file, n_new_file);
    }

    tpsetctxt(TPNULLCONTEXT, 0L);

    return;
}

/* vim: set ts=4 sw=4 et smartindent: */
