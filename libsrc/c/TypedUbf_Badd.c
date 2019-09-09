/**
 * @brief Java UBF Backing routines - add field to buffer
 *
 * @file TypedUbf_Badd.c
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
#include <jni.h>
#include <errno.h>
#include <stdlib.h>
#include <exjglue.h>
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
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, &clen, 
            NULL, EXFALSE, EXFALSE))
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
 * @param data TypedUbf buffer
 * @param bfldid field id
 * @param js short type
 */
expublic void JNICALL ndrxj_Java_org_endurox_TypedUbf_Badd__IS
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
expublic JNIEXPORT void JNICALL ndrxj_Java_org_endurox_TypedUbf_Badd__IJ
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
expublic JNIEXPORT void JNICALL ndrxj_Java_org_endurox_TypedUbf_Badd__IB
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
expublic JNIEXPORT void JNICALL ndrxj_Java_org_endurox_TypedUbf_Badd__IF
  (JNIEnv * env, jobject data, jint bfldid, jfloat jf)
{
    float f = (float)jf;
    ndrxj_ubf_CBadd(env, data, bfldid, (char *)&f, 0L, BFLD_FLOAT);
}

/**
 * Add double to UBF buffer
 * @param env java env
 * @param data UBF buffer
 * @param bfldid field id
 * @param jd float value
 */
expublic JNIEXPORT void JNICALL ndrxj_Java_org_endurox_TypedUbf_Badd__ID
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
expublic JNIEXPORT void JNICALL ndrxj_Java_org_endurox_TypedUbf_Badd__ILjava_lang_String_2
  (JNIEnv *env, jobject data, jint bfldid, jstring js)
{
    jboolean n_str_copy = EXFALSE;
    
    if (NULL!=js)
    {
        const char *n_str = (*env)->GetStringUTFChars(env, js, &n_str_copy);

        ndrxj_ubf_CBadd(env, data, bfldid, (char *)n_str, 0L, BFLD_STRING);

        if (n_str_copy)
        {
            (*env)->ReleaseStringUTFChars(env, js, n_str);
        }
    }
    else
    {
        /* throw exception due to NULL string */
        ndrxj_ubf_throw(env, BEINVAL, "%s: Failed to add %d (%s) - null value field", 
                __func__, bfldid, Bfname(bfldid));
    }
}

/**
 * Add byte array to buffer
 * @param env java env
 * @param data data buffer
 * @param bfldid field id
 * @param jb java byte array
 */
JNIEXPORT void JNICALL ndrxj_Java_org_endurox_TypedUbf_Badd__I_3B
  (JNIEnv * env, jobject data, jint bfldid, jbyteArray jb)
{
    jboolean n_carray_copy;
    char * n_carray = (char*)(*env)->GetByteArrayElements(env, jb, &n_carray_copy);
    jsize len = (*env)->GetArrayLength(env, jb);
    
    NDRX_LOG(log_error, "Adding carray len: %ld", (long)len);
    
    ndrxj_ubf_CBadd(env, data, bfldid, (char *)n_carray, (BFLDLEN)len, BFLD_CARRAY);
    
    if(n_carray_copy)
    {
       (*env)->ReleaseByteArrayElements(env, jb, n_carray, JNI_ABORT);
    }
}

/* vim: set ts=4 sw=4 et smartindent: */
