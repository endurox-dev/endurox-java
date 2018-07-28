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
JNIEXPORT void JNICALL Java_org_endurox_TypedUBF_Badd__IJ
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
JNIEXPORT void JNICALL Java_org_endurox_TypedUBF_Badd__IB
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
JNIEXPORT void JNICALL Java_org_endurox_TypedUBF_Badd__IF
  (JNIEnv * env, jobject data, jint bfldid, jfloat jf)
{
    float f = (long)jf;
    ndrxj_ubf_CBadd(env, data, bfldid, (char *)&f, 0L, BFLD_FLOAT);
}

/*
 * Class:     org_endurox_TypedUBF
 * Method:    Badd
 * Signature: (ID)V
 */
JNIEXPORT void JNICALL Java_org_endurox_TypedUBF_Badd__ID
  (JNIEnv *, jobject, jint, jdouble);

/*
 * Class:     org_endurox_TypedUBF
 * Method:    Badd
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_endurox_TypedUBF_Badd__ILjava_lang_String_2
  (JNIEnv *, jobject, jint, jstring);

/*
 * Class:     org_endurox_TypedUBF
 * Method:    Badd
 * Signature: (I[B)V
 */
JNIEXPORT void JNICALL Java_org_endurox_TypedUBF_Badd__I_3B
  (JNIEnv *, jobject, jint, jbyteArray);



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
    char *ret;
    short *s = NULL;
    
    /* exception will be thrown in case of failure */
    if (EXSUCCEED!=ndrxj_ubf_CBfind(env, data, bfldid, occ, &ret, NULL, BFLD_SHORT))
    {
        return (jshort)EXFAIL;
    }
    
    /* return the value */
    
    s = (short *)ret;
    return (jshort)*s;
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
