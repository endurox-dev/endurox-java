/**
 * @brief Java UBF Backing routines
 *
 * @file TypedUBF.c
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
#include "org_endurox_TypedBuffer.h"
#include "org_endurox_TypedUBF.h"
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
 * Common field adding routine 
 * @param env java env
 * @param data UBF buffer
 * @param bfldid filed id (compiled)
 * @param value field value
 * @param len value length
 * @param usrtype field type (see BFLD_*)
 */
exprivate void ndrxj_ubf_CBadd(JNIEnv *env, jobject data, jint bfldid, 
        char *value, BFLDLEN len, int usrtype)
{
    char *cdata;
    long clen;
    
    /* get the context, switch */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
       return; 
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, &clen))
    {
        NDRX_LOG(log_error, "Failed to get buffer data");
        goto out;
    }
    
    /* Set the field */
    
    if (EXSUCCEED!=CBadd((UBFH*)cdata, bfldid, value, len, usrtype))
    {
        UBF_LOG(log_error, "%s: CBadd failed to add %d (%s): %s", 
                __func__, bfldid, Bfname(bfldid), Bstrerror(Berror));
        ndrxj_ubf_throw(env, Berror, "%s: Failed to add %d (%s): %s", 
                __func__, bfldid, Bfname(bfldid), Bstrerror(Berror));
        goto out;
    }
    
out:
    
    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);
}

/**
 * Add field to buffer, short type
 * @param env java env
 * @param data TypedUBF buffer
 * @param bfldid field id
 * @param js short type
 */
expublic void JNICALL Java_org_endurox_TypedUBF_Badd__IS
  (JNIEnv *env, jobject data, jint bfldid, jshort js)
{
   short s = (short)js;
   ndrxj_ubf_CBadd(env, data, bfldid, (char *)&s, 0L, BFLD_SHORT);
}

/**
 * Add long to UBF buffer
 * @param env java env
 * @param data ubf buffer
 * @param bfldid field id
 * @param jl long value
 */
expublic JNIEXPORT void JNICALL Java_org_endurox_TypedUBF_Badd__IJ
  (JNIEnv * env, jobject data, jint bfldid, jlong jl)
{
    long l = (long)jl;
    ndrxj_ubf_CBadd(env, data, bfldid, (char *)&l, 0L, BFLD_LONG);
}

/**
 * Add byte/char to buffer
 * @param env java env
 * @param data data buffer ubf
 * @param bfldid field id
 * @param jb char to add
 */
expublic JNIEXPORT void JNICALL Java_org_endurox_TypedUBF_Badd__IB
  (JNIEnv * env, jobject data, jint bfldid, jbyte jb)
{
    char c = (long)jb;
    ndrxj_ubf_CBadd(env, data, bfldid, (char *)&c, 0L, BFLD_CHAR);
}

/**
 * Add float to UBF buffer
 * @param env java env
 * @param data UBF buffer
 * @param bfldid field id
 * @param jf float value
 */
expublic JNIEXPORT void JNICALL Java_org_endurox_TypedUBF_Badd__IF
  (JNIEnv * env, jobject data, jint bfldid, jfloat jf)
{
    float f = (long)jf;
    ndrxj_ubf_CBadd(env, data, bfldid, (char *)&f, 0L, BFLD_FLOAT);
}

/**
 * Add double to UBF buffer
 * @param env java env
 * @param data UBF buffer
 * @param bfldid field id
 * @param jd float value
 */
expublic JNIEXPORT void JNICALL Java_org_endurox_TypedUBF_Badd__ID
  (JNIEnv * env, jobject data, jint bfldid, jdouble jd)
{
    double d = (double)jd;
    ndrxj_ubf_CBadd(env, data, bfldid, (char *)&d, 0L, BFLD_DOUBLE);
}

/**
 * Add string to UBF buffer
 * @param env java env
 * @param data data buffer
 * @param bfldid field id
 * @param js java string
 */
expublic JNIEXPORT void JNICALL Java_org_endurox_TypedUBF_Badd__ILjava_lang_String_2
  (JNIEnv *env, jobject data, jint bfldid, jstring js)
{
    jboolean n_str_copy = EXFALSE;
    const char *n_str = (*env)->GetStringUTFChars(env, js, &n_str_copy);
    
    ndrxj_ubf_CBadd(env, data, bfldid, (char *)n_str, 0L, BFLD_STRING);
    
    if (n_str_copy)
    {
        (*env)->ReleaseStringUTFChars(env, js, n_str);
    }
}

/**
 * Add byte array to buffer
 * @param env java env
 * @param data data buffer
 * @param bfldid field id
 * @param jb java byte array
 */
JNIEXPORT void JNICALL Java_org_endurox_TypedUBF_Badd__I_3B
  (JNIEnv * env, jobject data, jint bfldid, jbyteArray jb)
{
    jboolean n_carray_copy;
    char * n_carray = (char*)(*env)->GetByteArrayElements(env , jb, &n_carray_copy);
    
    if(n_carray_copy)
    {
       (*env)->ReleaseByteArrayElements(env, jb, n_carray, JNI_ABORT);
    }
}

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
        UBF_LOG(log_error, "%s: CBfind failed to add %d (%s) occ %d: %s", 
                __func__, bfldid, Bfname(bfldid), (int)occ, Bstrerror(Berror));
        
        ndrxj_ubf_throw(env, Berror, "%s: Failed to add %d (%s) occ %d: %s", 
                __func__, bfldid, Bfname(bfldid), (int)occ, Bstrerror(Berror));
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
expublic JNIEXPORT jshort JNICALL Java_org_endurox_TypedUBF_BgetShort
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
JNIEXPORT jlong JNICALL Java_org_endurox_TypedUBF_BgetLong
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
JNIEXPORT jbyte JNICALL Java_org_endurox_TypedUBF_BgetByte
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
JNIEXPORT jfloat JNICALL Java_org_endurox_TypedUBF_BgetFloat
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
JNIEXPORT jdouble JNICALL Java_org_endurox_TypedUBF_BgetDouble
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
JNIEXPORT jstring JNICALL Java_org_endurox_TypedUBF_BgetString
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
JNIEXPORT jbyteArray JNICALL Java_org_endurox_TypedUBF_BgetByteArr
  (JNIEnv * env, jobject data, jint bfldid, jint occ)
{
    char *buf;
    BFLDLEN len;
    jbyteArray ret = NULL;
    
    /* exception will be thrown in case of failure */
    if (EXSUCCEED!=ndrxj_ubf_CBfind(env, data, bfldid, occ, &buf, &len, BFLD_STRING))
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

/**
 * Print the UBF buffer to STDOUT
 * @param env java env
 * @param data TypedUBF object
 */
expublic void JNICALL Java_org_endurox_TypedUBF_Bprint(JNIEnv *env, jobject data)
{
    char *cdata;
    long clen;
    
    /* get the context, switch */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
       return; 
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, &clen))
    {
        NDRX_LOG(log_error, "Failed to get buffer data");
        goto out;
    }
    
    if (EXSUCCEED!=Bprint((UBFH *)cdata))
    {
        UBF_LOG(log_error, "%s: failed to Bprint %p buffer: %s", 
                __func__, cdata, Bstrerror(Berror));
        ndrxj_ubf_throw(env, Berror, "%s: failed to Bprint %p buffer: %s", 
                __func__, cdata, Bstrerror(Berror));
        goto out;
    }
    
out:
    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);
}

/* vim: set ts=4 sw=4 et cindent: */
