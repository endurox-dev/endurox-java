/**
 * @brief Java UBF Backing routines
 *
 * @file TypedUbf_Baddfast.c
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
#include <exjglue.h>
#include <atmi.h>
#include <oatmi.h>
#include <ndebug.h>
#include <ondebug.h>
#include <oatmisrv_integra.h>
#include <libsrc.h>
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
 * @param fldloc field location infos
 */
exprivate void ndrxj_ubf_Baddfast(JNIEnv *env, jobject data, jint bfldid, 
        char *value, BFLDLEN len, int usrtype, jobject jfldloc)
{
    char *cdata;
    long clen;
    Bfld_loc_info_t loc;
    BFLDID* ptr;
    BFLDID last_Baddfast;
    
    /* get the context, switch */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
       return; 
    }
    
    /* check types */
    if (Bfldtype(bfldid) !=usrtype)
    {
        /* Throw exception ! */
        ndrxj_ubf_throw(env, BEINVAL, "%s: Invalid field type passed, for type "
                "%d func got type %d field (%d) must be the same", 
                __func__, usrtype, Bfldtype(usrtype), (int)usrtype);
        goto out;
    }
    
    ptr = ndrxj_BFldLocInfo_ptr_get(env, jfldloc, &last_Baddfast);
    
    /* the ptr might be a NULL, thats fine, but to continue we shall check
     * for exception
     */
    
    if ((*env)->ExceptionCheck(env))
    {
        goto out;
    }
    
    loc.last_checked = ptr;
    loc.last_Baddfast = last_Baddfast;
    
    /* get UBF buffer */
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, &clen,
            NULL, EXFALSE, EXFALSE))
    {
        NDRX_LOG(log_error, "Failed to get buffer data");
        goto out;
    }
    
    /* Set the field */
    
    if (EXSUCCEED!=Baddfast((UBFH*)cdata, bfldid, value, len, &loc))
    {
        UBF_LOG(log_error, "%s: Baddfast failed to add %d (%s): %s", 
                __func__, bfldid, Bfname(bfldid), Bstrerror(Berror));
        ndrxj_ubf_throw(env, Berror, "%s: Failed to add %d (%s): %s", 
                __func__, bfldid, Bfname(bfldid), Bstrerror(Berror));
        goto out;
    }
    
    /* save location */
    ndrxj_BFldLocInfo_ptr_set(env, jfldloc, loc.last_checked, loc.last_Baddfast);
    
out:
    
    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);
}

/**
 * Fast add field to UBF buffer.
 * Also note that types must match otherwise the exception will be thrown.
 * @param env java env
 * @param data UBF buffer
 * @param bfldid compiled field id
 * @param js java short value
 * @param fldloc field location holder
 */
expublic JNIEXPORT void JNICALL ndrxj_Java_org_endurox_TypedUbf_Baddfast__ISLorg_endurox_BFldLocInfo_2
  (JNIEnv * env, jobject data, jint bfldid, jshort js, jobject fldloc) 
{
    
    short s = (short)js;
    /* check types if not short the field id, then throw exception */
    
    ndrxj_ubf_Baddfast(env, data, bfldid, 
        (char *)&s, 0L, BFLD_SHORT, fldloc);
    
}

/**
 * Fast add field to UBF buffer.
 * Also note that types must match otherwise the exception will be thrown.
 * @param env java env
 * @param data UBF buffer
 * @param bfldid compiled field id
 * @param js java long value
 * @param fldloc field location holder
 */
expublic void JNICALL ndrxj_Java_org_endurox_TypedUbf_Baddfast__IJLorg_endurox_BFldLocInfo_2
  (JNIEnv * env, jobject data, jint bfldid, jlong jl, jobject fldloc)
{
    long l = (long)jl;
    /* check types if not short the field id, then throw exception */
    
    ndrxj_ubf_Baddfast(env, data, bfldid, 
        (char *)&l, 0L, BFLD_LONG, fldloc);
}

/**
 * Fast add single byte char to UBF buffer
 * @param env java env
 * @param data UBF buffer
 * @param bfldid compiled field id
 * @param js java long value
 * @param fldloc field location holder
 */
expublic JNIEXPORT void JNICALL ndrxj_Java_org_endurox_TypedUbf_Baddfast__IBLorg_endurox_BFldLocInfo_2
  (JNIEnv * env, jobject data, jint bfldid, jbyte jb, jobject fldloc)
{
    char c = (char)jb;
    ndrxj_ubf_Baddfast(env, data, bfldid, 
        (char *)&c, 0L, BFLD_CHAR, fldloc);
}

/**
 * Add data to UBF buffer, float
 * @param env java env
 * @param data UBF buffer
 * @param bfldid compiled field id
 * @param jf java float value
 * @param fldloc field location holder
 */
expublic JNIEXPORT void JNICALL ndrxj_Java_org_endurox_TypedUbf_Baddfast__IFLorg_endurox_BFldLocInfo_2
  (JNIEnv * env, jobject data, jint bfldid, jfloat jf, jobject fldloc)
{
    float f = (float)jf;
    ndrxj_ubf_Baddfast(env, data, bfldid, 
        (char *)&f, 0L, BFLD_FLOAT, fldloc);
}

/**
 * Add data to UBF buffer, double
 * @param env java env
 * @param data UBF buffer
 * @param bfldid compiled field id
 * @param jd double value, java
 * @param fldloc field location holder
 */
expublic JNIEXPORT void JNICALL ndrxj_Java_org_endurox_TypedUbf_Baddfast__IDLorg_endurox_BFldLocInfo_2
  (JNIEnv * env, jobject data, jint bfldid, jdouble jd, jobject fldloc)
{
    double d = (double)jd;
    ndrxj_ubf_Baddfast(env, data, bfldid, 
        (char *)&d, 0L, BFLD_DOUBLE, fldloc);
}

/**
 * Add data to UBF buffer, string
 * @param env java env
 * @param data UBF buffer
 * @param bfldid compiled field id
 * @param js java string value
 * @param fldloc field location holder
 */
expublic JNIEXPORT void JNICALL ndrxj_Java_org_endurox_TypedUbf_Baddfast__ILjava_lang_String_2Lorg_endurox_BFldLocInfo_2
  (JNIEnv * env, jobject data, jint bfldid, jstring js, jobject fldloc)
{
    jboolean n_str_copy = EXFALSE;
    const char *n_str = (*env)->GetStringUTFChars(env, js, &n_str_copy);
    
    ndrxj_ubf_Baddfast(env, data, bfldid, 
        (char *)n_str, 0L, BFLD_STRING, fldloc);
    
    if (n_str_copy)
    {
        (*env)->ReleaseStringUTFChars(env, js, n_str);
    }
}

/**
 * Add data to UBF buffer, carray
 * @param env java env
 * @param data UBF buffer
 * @param bfldid compiled field id
 * @param jb java byte array value
 * @param fldloc field location holder
 */
expublic JNIEXPORT void JNICALL ndrxj_Java_org_endurox_TypedUbf_Baddfast__I_3BLorg_endurox_BFldLocInfo_2
  (JNIEnv * env, jobject data, jint bfldid, jbyteArray jb, jobject fldloc)
{
    jboolean n_carray_copy;
    char * n_carray = (char*)(*env)->GetByteArrayElements(env, jb, &n_carray_copy);
    jsize len = (*env)->GetArrayLength(env, jb);
    
    NDRX_LOG(log_error, "Adding carray len: %ld", (long)len);
    
    ndrxj_ubf_Baddfast(env, data, bfldid, (char *)n_carray, 
            (BFLDLEN)len, BFLD_CARRAY, fldloc);
    
    if(n_carray_copy)
    {
       (*env)->ReleaseByteArrayElements(env, jb, (signed char *)n_carray, JNI_ABORT);
    }
}

/* vim: set ts=4 sw=4 et smartindent: */
