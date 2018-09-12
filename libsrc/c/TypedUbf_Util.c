/**
 * @brief Utility lib (various small UBF parts)
 *
 * @file TypedUbf_Util.c
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
 * Test field presence in UBF buffer
 * @param env java env
 * @param data UBF buffer object
 * @param bfldid compiled buffer id
 * @param occ field occurrence
 * @return JNI_TRUE in case if field present, JNI_FALSE field does not exists or
 *     error occurred.
 */
expublic jboolean JNICALL Java_org_endurox_TypedUbf_Bpres
  (JNIEnv *env, jobject data, jint bfldid, jint occ)
{
    char *cdata;
    long clen;
    jboolean ret = JNI_FALSE;
    
    /* get the context, switch */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
        return ret;
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, &clen))
    {
        UBF_LOG(log_error, "Failed to get buffer data");
        goto out;
    }
    
    /* Delete the field */
    
    ret = (jboolean) Bpres((UBFH*)cdata, (BFLDID)bfldid, (BFLDOCC)occ);
    
out:
    
    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);

    return ret;
}

/**
 * Test the buffer correspondence to the UBF format
 * @param env java env
 * @param data UBF buffer object
 * @return JNI_TRUE if buffer is UBF, JNI_FALSE buffer is not UBF.
 */
expublic jboolean JNICALL Java_org_endurox_TypedUbf_Bisubf
  (JNIEnv * env, jobject data)
{
    char *cdata;
    long clen;
    jboolean ret = JNI_FALSE;
    
    /* get the context, switch */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
        return ret;
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, &clen))
    {
        UBF_LOG(log_error, "Failed to get buffer data");
        goto out;
    }
    
    /* Delete the field */
    
    ret = (jboolean) Bisubf((UBFH*)cdata);
    
out:
    
    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);

    return ret;
}

/**
 * Return field length in bytes
 * @param env java env
 * @param data UBF buffer
 * @param bfldid compiled field id
 * @param occ field occurrence
 * @return field length in bytes
 */
expublic jint JNICALL Java_org_endurox_TypedUbf_Blen
  (JNIEnv *env, jobject data, jint bfldid, jint occ)
{
    char *cdata;
    long clen;
    jint ret = EXFAIL;
    
    /* get the context, switch */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
        return ret;
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, &clen))
    {
        UBF_LOG(log_error, "Failed to get buffer data");
        goto out;
    }
    
    /* Delete the field */
    ret = (jint) Blen((UBFH*)cdata, (BFLDID)bfldid, (BFLDOCC)occ);
    
out:
    
    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);

    return ret;
}

/**
 * Loop over the UBF buffer
 * @param env java env
 * @param data UBF buffer
 * @param first do we restart the iteration?
 * @return BNextResult
 */
expublic jobject JNICALL Java_org_endurox_TypedUbf_Bnext
  (JNIEnv *env, jobject data, jboolean first)
{
    char *cdata;
    long clen;
    jobject ret = NULL;
    int retBnext;
    BFLDID bfldid;
    BFLDOCC occ;
    BFLDLEN len = 0;
    
    /* get the context, switch */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
        return ret;
    }
    
    if (first)
    {
        bfldid = BFIRSTFLDID;
    }
    else
    {
        /* continue to loop... */
        bfldid = BFIRSTFLDID+1;
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, &clen))
    {
        UBF_LOG(log_error, "Failed to get buffer data");
        goto out;
    }
    
    retBnext=Bnext ((UBFH *)cdata, &bfldid, &occ, NULL, &len);
    
    if (EXFAIL==retBnext)
    {
        /* throw exception */
        ndrxj_ubf_throw(env, Berror, "%s: failed to Bnext %p buffer: %s", 
                __func__, cdata, Bstrerror(Berror));
        goto out;
    }
    else if (0==retBnext)
    {
        /* EOF reached */
        UBF_LOG(log_debug, "jeof");
        goto out;
    }
    
    /* Build up the response object with
     * with bfldid, occ, len 
     */
    ret = ndrxj_BNextResult_new(env, bfldid, occ, len);
    
out:
    
    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);

    return ret;
}

/**
 * Get number of occurrences of the field in UBF buffer
 * @param env java env
 * @param data UBF buffer
 * @param bfldid compiled field id
 * @return number of occurrences of field
 */
expublic jint JNICALL Java_org_endurox_TypedUbf_Boccur
  (JNIEnv *env, jobject data, jint bfldid)
{
    char *cdata;
    long clen;
    jint ret = EXFAIL;
    
    /* get the context, switch */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
        return ret;
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, &clen))
    {
        UBF_LOG(log_error, "Failed to get buffer data");
        goto out;
    }
    
    /* Delete the field */
    ret = (jint) Boccur((UBFH*)cdata, (BFLDID)bfldid);
    
out:
    
    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);

    return ret;
}

/**
 * Return total size of the allocated buffer size in bytes
 * @param env java env
 * @param data UBF buffer object
 * @return size in bytes
 */
expublic jlong JNICALL Java_org_endurox_TypedUbf_Bsizeof
  (JNIEnv * env, jobject data)
{
    char *cdata;
    long clen;
    jlong ret = EXFAIL;
    
    /* get the context, switch */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
        return ret;
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, &clen))
    {
        UBF_LOG(log_error, "Failed to get buffer data");
        goto out;
    }
    
    /* Delete the field */
    ret = (jlong) Bsizeof((UBFH*)cdata);
    
out:
    
    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);

    return ret;
}

/**
 * Return free space of UBF buffer
 * @param env java env
 * @param data UBF data
 * @return free space in bytes
 */
expublic jlong JNICALL Java_org_endurox_TypedUbf_Bunused
  (JNIEnv * env, jobject data)
{
    char *cdata;
    long clen;
    jlong ret = EXFAIL;
    
    /* get the context, switch */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
        return ret;
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, &clen))
    {
        UBF_LOG(log_error, "Failed to get buffer data");
        goto out;
    }
    
    /* Delete the field */
    ret = (jlong) Bunused((UBFH*)cdata);
    
out:
    
    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);

    return ret;
}

/**
 * Used bytes in UBF buffer
 * @param env java env
 * @param data UBF buffer obj
 * @return used bytes
 */
expublic jlong JNICALL Java_org_endurox_TypedUbf_Bused
  (JNIEnv * env, jobject data)
{
    char *cdata;
    long clen;
    jlong ret = EXFAIL;
    
    /* get the context, switch */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
        return ret;
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, &clen))
    {
        UBF_LOG(log_error, "Failed to get buffer data");
        goto out;
    }
    
    /* Delete the field */
    ret = (jlong) Bused((UBFH*)cdata);
    
out:
    
    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);

    return ret;
}

/**
 * Reset/Initialize UBF buffer
 * @param env java env
 * @param data UBF buffer
 * @return total size of the buffer
 */
expublic jlong JNICALL Java_org_endurox_TypedUbf_Binit
  (JNIEnv *env, jobject data)
{
    char *cdata;
    long clen;
    jlong ret = EXFAIL;
    
    /* get the context, switch */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
        return ret;
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, &clen))
    {
        UBF_LOG(log_error, "Failed to get buffer data");
        goto out;
    }
    
    /* get size of the buffer */
    ret = (jlong) Bsizeof((UBFH*)cdata);
    
    if (EXFAIL==ret)
    {
        /* throw exception */
        ndrxj_ubf_throw(env, Berror, "%s: Failed to get %p buffer size: %s", 
                __func__, cdata, Bstrerror(Berror));
        goto out;        
    }
    
    if (EXSUCCEED!=Binit((UBFH *)cdata, (BFLDLEN)ret))
    {
        /* throw exception */
        ndrxj_ubf_throw(env, Berror, "%s: Failed to init %p buffer to size %d: %s", 
                __func__, cdata, (int)ret, Bstrerror(Berror));
        goto out;       
    }
    
out:
    
    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);

    return ret;
}

/*
 * TODO:
 * executed on DEST object:
BConcat
BCpy
BInit
BProjCpy
BUpdate
 
 */

/* vim: set ts=4 sw=4 et cindent: */
