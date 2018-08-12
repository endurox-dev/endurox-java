/**
 * @brief Change field value in the buffer
 *
 * @file TypedUBF_Bchg.c
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
 * Common field changing routine 
 * @param env java env
 * @param data UBF buffer
 * @param bfldid filed id (compiled)
 * @param occ filed occurrence
 * @param value field value
 * @param len value length
 * @param usrtype field type (see BFLD_*)
 */
exprivate void ndrxj_ubf_CBchg(JNIEnv *env, jobject data, jint bfldid, jint occ,
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
    
    if (EXSUCCEED!=CBchg((UBFH*)cdata, bfldid, (BFLDOCC)occ, value, len, usrtype))
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

#if 0

/*
 * Class:     org_endurox_TypedUBF
 * Method:    Bchg
 * Signature: (IIS)V
 */
JNIEXPORT void JNICALL Java_org_endurox_TypedUBF_Bchg__IIS
  (JNIEnv *, jobject, jint, jint, jshort);

/*
 * Class:     org_endurox_TypedUBF
 * Method:    Bchg
 * Signature: (IIJ)V
 */
JNIEXPORT void JNICALL Java_org_endurox_TypedUBF_Bchg__IIJ
  (JNIEnv *, jobject, jint, jint, jlong);

/*
 * Class:     org_endurox_TypedUBF
 * Method:    Bchg
 * Signature: (IIB)V
 */
JNIEXPORT void JNICALL Java_org_endurox_TypedUBF_Bchg__IIB
  (JNIEnv *, jobject, jint, jint, jbyte);

/*
 * Class:     org_endurox_TypedUBF
 * Method:    Bchg
 * Signature: (IIF)V
 */
JNIEXPORT void JNICALL Java_org_endurox_TypedUBF_Bchg__IIF
  (JNIEnv *, jobject, jint, jint, jfloat);

/*
 * Class:     org_endurox_TypedUBF
 * Method:    Bchg
 * Signature: (IID)V
 */
JNIEXPORT void JNICALL Java_org_endurox_TypedUBF_Bchg__IID
  (JNIEnv *, jobject, jint, jint, jdouble);

/*
 * Class:     org_endurox_TypedUBF
 * Method:    Bchg
 * Signature: (IILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_endurox_TypedUBF_Bchg__IILjava_lang_String_2
  (JNIEnv *, jobject, jint, jint, jstring);

/*
 * Class:     org_endurox_TypedUBF
 * Method:    Bchg
 * Signature: (II[B)V
 */
JNIEXPORT void JNICALL Java_org_endurox_TypedUBF_Bchg__II_3B
  (JNIEnv *, jobject, jint, jint, jbyteArray);



#endif

/**
 * Change field, short type
 * @param env java env
 * @param data TypedUBF buffer
 * @param bfldid field id
 * @param occ field occurrence
 * @param js short type
 */
expublic void JNICALL Java_org_endurox_TypedUBF_Bchg__IIS
(JNIEnv *env, jobject data, jint bfldid, jint occ, jshort js)
{
   short s = (short)js;
   ndrxj_ubf_CBchg(env, data, bfldid, occ, (char *)&s, 0L, BFLD_SHORT);
}

/**
 * Change long field
 * @param env java env
 * @param data ubf buffer
 * @param bfldid field id
 * @param occ occurrence to change
 * @param jl long value
 */
expublic JNIEXPORT void Java_org_endurox_TypedUBF_Bchg__IIJ
  (JNIEnv * env, jobject data, jint bfldid, jint occ, jlong jl)
{
    long l = (long)jl;
    ndrxj_ubf_CBchg(env, data, bfldid, occ, (char *)&l, 0L, BFLD_LONG);
}

/**
 * Change char/byte field
 * @param env java env
 * @param data data buffer ubf
 * @param bfldid field id
 * @param occ occurrence
 * @param jb char to add
 */
expublic JNIEXPORT void JNICALL Java_org_endurox_TypedUBF_Bchg__IIB
  (JNIEnv * env, jobject data, jint bfldid, jint occ, jbyte jb)
{
    char c = (long)jb;
    ndrxj_ubf_CBchg(env, data, bfldid, occ, (char *)&c, 0L, BFLD_CHAR);
}

/**
 * Change float field
 * @param env java env
 * @param data UBF buffer
 * @param bfldid field id
 * @param occ occurrence
 * @param jf float value
 */
expublic JNIEXPORT void JNICALL Java_org_endurox_TypedUBF_Bchg__IIF
  (JNIEnv * env, jobject data, jint bfldid, jint occ, jfloat jf)
{
    float f = (float)jf;
    ndrxj_ubf_CBchg(env, data, bfldid, occ, (char *)&f, 0L, BFLD_FLOAT);
}

/**
 * Change double field
 * @param env java env
 * @param data UBF buffer
 * @param bfldid field id
 * @param occ occurrence
 * @param jd float value
 */
expublic JNIEXPORT void JNICALL Java_org_endurox_TypedUBF_Bchg__IID
  (JNIEnv * env, jobject data, jint bfldid, jint occ, jdouble jd)
{
    double d = (double)jd;
    ndrxj_ubf_CBchg(env, data, bfldid, occ, (char *)&d, 0L, BFLD_DOUBLE);
}

/**
 * Change string field
 * @param env java env
 * @param data data buffer
 * @param bfldid field id
 * @param occ occurrence
 * @param js java string
 */
expublic JNIEXPORT void JNICALL Java_org_endurox_TypedUBF_Bchg__IILjava_lang_String_2
  (JNIEnv *env, jobject data, jint bfldid, jint occ, jstring js)
{
    jboolean n_str_copy = EXFALSE;
    const char *n_str = (*env)->GetStringUTFChars(env, js, &n_str_copy);
    
    ndrxj_ubf_CBchg(env, data, bfldid, occ, (char *)n_str, 0L, BFLD_STRING);
    
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
 * @param occ occurrence
 * @param jb java byte array
 */
JNIEXPORT void JNICALL Java_org_endurox_TypedUBF_Bchg__II_3B
  (JNIEnv * env, jobject data, jint bfldid,  jint occ, jbyteArray jb)
{
    jboolean n_carray_copy;
    char * n_carray = (char*)(*env)->GetByteArrayElements(env, jb, &n_carray_copy);
    jsize len = (*env)->GetArrayLength(env, jb);
    
    NDRX_LOG(log_error, "Adding carray len: %ld", (long)len);
    
    ndrxj_ubf_CBchg(env, data, bfldid, occ, (char *)n_carray, (BFLDLEN)len, 
            BFLD_CARRAY);
    
    if(n_carray_copy)
    {
       (*env)->ReleaseByteArrayElements(env, jb, n_carray, JNI_ABORT);
    }
}

/* vim: set ts=4 sw=4 et cindent: */
