/**
 * @brief Java UBF Backing routines
 *
 * @file TypedUbf_Bget.c
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
#include "org_endurox_TypedBuffer.h"
#include "org_endurox_TypedUbf.h"
#include <atmi.h>
#include <oatmi.h>
#include <ndebug.h>
#include <ondebug.h>
#include <oatmisrv_integra.h>
#include "libsrc.h"
#include <sys_unix.h>
#include "nerror.h"
#include <ndrstandard.h>
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/

/**
 * Find field value in buffer
 * @param env Java env
 * @param data UBF data buffer
 * @param bfldid field id 
 * @param occ occurrence to find
 * @param value field value to return double ptr
 * @param len field length 
 * @param usrtype user try expected (cast to if needed)
 * @return EXSUCCEED/EXFAIL
 */
exprivate int ndrxj_ubf_CBfind(JNIEnv *env, jobject data, jint bfldid, BFLDOCC occ,
        char **value, BFLDLEN *len, int usrtype)
{
    char *cdata;
    long clen;
    int ret = EXSUCCEED;
    
    /* get the context, switch */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
        EXFAIL_OUT(ret);
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, &clen))
    {
        NDRX_LOG(log_error, "Failed to get buffer data");
        EXFAIL_OUT(ret);
    }
    
    /* Get the field value */
    
    if (NULL==(*value = CBfind((UBFH*)cdata, bfldid, occ, len, usrtype)))
    {
        int err = Berror;
        UBF_LOG(log_error, "%s: CBfind failed to find %d (%s) occ %d: %s", 
                __func__, bfldid, Bfname(bfldid), (int)occ, Bstrerror(err));
        
        ndrxj_ubf_throw(env, err, "%s: Failed to find %d (%s) occ %d: %s", 
                __func__, bfldid, Bfname(bfldid), (int)occ, Bstrerror(err));
        EXFAIL_OUT(ret);
    }
    
out:
    
    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);

    return ret;
}

/**
 * Return short field value from buffer
 * @param env Java env
 * @param data UBF data buffer
 * @param bfldid compiled field
 * @param occ field occurrence 
 * @return short value
 * @throws any UBF
 */
expublic JNIEXPORT jshort JNICALL Java_org_endurox_TypedUbf_BgetShort
  (JNIEnv * env, jobject data, jint bfldid, jint occ)
{
    char *buf;
    short *v = NULL;
    
    /* exception will be thrown in case of failure */
    if (EXSUCCEED!=ndrxj_ubf_CBfind(env, data, bfldid, occ, &buf, NULL, BFLD_SHORT))
    {
        return (jshort)EXFAIL;
    }
    
    /* return the value */
    v = (short *)buf;
    return (jshort)*v;
}

/**
 * get long value from buffer
 * @param env Java env
 * @param data UBF data buffer
 * @param bfldid compiled field
 * @param occ field occurrence 
 * @return long value
 * @throws any UBF
 */
JNIEXPORT jlong JNICALL Java_org_endurox_TypedUbf_BgetLong
      (JNIEnv * env, jobject data, jint bfldid, jint occ)
{
    char *buf;
    long *v = NULL;
    
    /* exception will be thrown in case of failure */
    if (EXSUCCEED!=ndrxj_ubf_CBfind(env, data, bfldid, occ, &buf, NULL, BFLD_LONG))
    {
        return (jlong)EXFAIL;
    }
    
    /* return the value */
    v = (long *)buf;
    return (jlong)*v;
}

/**
 * Get byte/ansi char from buffer
 * @param env Java env
 * @param data UBF data buffer
 * @param bfldid compiled field
 * @param occ field occurrence 
 * @return char value
 * @throws any UBF
 */
JNIEXPORT jbyte JNICALL Java_org_endurox_TypedUbf_BgetByte
  (JNIEnv * env, jobject data, jint bfldid, jint occ)
{
    char *buf;
    char *v = NULL;
    
    /* exception will be thrown in case of failure */
    if (EXSUCCEED!=ndrxj_ubf_CBfind(env, data, bfldid, occ, &buf, NULL, BFLD_CHAR))
    {
        return (jbyte)EXFAIL;
    }
    
    /* return the value */
    v = (char *)buf;
    
    return (jbyte)*v;
}

/**
 * Get float value from buffer
 * @param env Java env
 * @param data UBF data buffer
 * @param bfldid compiled field
 * @param occ field occurrence 
 * @return float value
 * @throws any UBF
 */
JNIEXPORT jfloat JNICALL Java_org_endurox_TypedUbf_BgetFloat
  (JNIEnv * env, jobject data, jint bfldid, jint occ)
{
    char *buf;
    float *v = NULL;
    
    /* exception will be thrown in case of failure */
    if (EXSUCCEED!=ndrxj_ubf_CBfind(env, data, bfldid, occ, &buf, NULL, BFLD_FLOAT))
    {
        return (jfloat)EXFAIL;
    }
    
    /* return the value */
    v = (float *)buf;
    return (jfloat)*v;
}

/**
 * Get double value from buffer
 * @param env Java env
 * @param data UBF data buffer
 * @param bfldid compiled field
 * @param occ field occurrence 
 * @return double value
 * @throws any UBF
 */
JNIEXPORT jdouble JNICALL Java_org_endurox_TypedUbf_BgetDouble
  (JNIEnv * env, jobject data, jint bfldid, jint occ)
{
    char *buf;
    double *v = NULL;
    
    /* exception will be thrown in case of failure */
    if (EXSUCCEED!=ndrxj_ubf_CBfind(env, data, bfldid, occ, &buf, NULL, BFLD_DOUBLE))
    {
        return (jdouble)EXFAIL;
    }
    
    /* return the value */
    v = (double *)buf;
    return (jdouble)*v;
}

/**
 * Return string from buffer
 * @param env Java env
 * @param data UBF data buffer
 * @param bfldid compiled field
 * @param occ field occurrence 
 * @return string value
 * @throws any UBF
 */
JNIEXPORT jstring JNICALL Java_org_endurox_TypedUbf_BgetString
  (JNIEnv * env, jobject data, jint bfldid, jint occ)
{
    char *buf;
    
    /* exception will be thrown in case of failure */
    if (EXSUCCEED!=ndrxj_ubf_CBfind(env, data, bfldid, occ, &buf, NULL, BFLD_STRING))
    {
        return (jstring)NULL;
    }
    /* build java string... */
    return (*env)->NewStringUTF(env, buf);
}

/**
 * Return byte array from ubf buffer
 * @param env Java env
 * @param data UBF data buffer
 * @param bfldid compiled field
 * @param occ field occurrence 
 * @return java byte array value
 * @throws any UBF
 */
JNIEXPORT jbyteArray JNICALL Java_org_endurox_TypedUbf_BgetByteArr
  (JNIEnv * env, jobject data, jint bfldid, jint occ)
{
    char *buf;
    BFLDLEN len = 0;
    jbyteArray ret = NULL;
    
    /* exception will be thrown in case of failure */
    if (EXSUCCEED!=ndrxj_ubf_CBfind(env, data, bfldid, occ, &buf, &len, BFLD_CARRAY))
    {
        return (jstring)NULL;
    }
    
    ret = (*env)->NewByteArray(env, (jsize)len);

    if (NULL==ret)
    {
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_ULOG, 
                "Failed to create byte array with: %s, size: %d", (int)len);
        goto out;
    }

    (*env)->SetByteArrayRegion(env, ret, 0, len, 
                            (jbyte*)buf);

    if((*env)->ExceptionCheck(env))
    {
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_ULOG, 
                "Failed to set data bytes: %s");
        goto out;
    }
out:
    return ret;
}

/* vim: set ts=4 sw=4 et smartindent: */
