/**
 * @brief Projection related backends
 *
 * @file TypedUbf_Proj.c
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
 * Perform projection on UBF buffer - i.e. leave only field in the array
 * @param env java env
 * @param data UBF buffer
 * @param bfldida list of fields to leave in buffer
 */
expublic void JNICALL Java_org_endurox_TypedUbf_Bproj
  (JNIEnv * env, jobject data, jintArray bfldida)
{
    char *cdata;
    long clen;
    BFLDID *barra = NULL;
    int i;
    int len;
    jboolean jarr_copy = JNI_FALSE;
    jint *jarr;
    
    /* get the context, switch */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
       return; 
    }
    
    len = (*env)->GetArrayLength(env, bfldida);
    
    UBF_LOG(log_debug, "%s: Number of fields to copy: %d", __func__, len);
    
    barra = NDRX_MALLOC(sizeof(BFLDID)*(len+1));
    
    if (NULL==barra)
    {
        int err = errno;
        UBF_LOG(log_error, "%s: Failed to malloc %d bytes: %s", 
                __func__, (int)(sizeof(BFLDID)*(len+1)), strerror(err));
        userlog("%s: Failed to malloc %d bytes: %s", 
                __func__, (int)(sizeof(BFLDID)*(len+1)), strerror(err));
        ndrxj_ubf_throw(env, BEUNIX, "%s: Failed to malloc %d bytes: %s", 
                __func__, (int)(sizeof(BFLDID)*(len+1)), strerror(err));
        goto out;
    }
    
    jarr = (*env)->GetIntArrayElements(env, bfldida, &jarr_copy);
    
    for (i=0; i<len; i++)
    {
        barra[i] = (BFLDID)jarr[i];
    }
    
    barra[i] = BBADFLDID;
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, &clen))
    {
        UBF_LOG(log_error, "Failed to get buffer data");
        goto out;
    }
    
    /* Delete the field */
    
    if (EXSUCCEED!=Bproj((UBFH*)cdata, barra))
    {
        UBF_LOG(log_error, "%s: Bproj %p: %s", 
                __func__, barra, Bstrerror(Berror));
        ndrxj_ubf_throw(env, Berror, "%s: Bproj %p: %s", 
                __func__, barra, Bstrerror(Berror));
        goto out;
    }
    
out:
    
    if (jarr_copy)
    {
        (*env)->ReleaseIntArrayElements(env, bfldida, jarr, JNI_ABORT);
    }

    if (NULL!=barra)
    {
        NDRX_FREE(barra);
    }

    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);

}

/**
 * Contactenate two UBF buffer
 * @param env java env
 * @param data dest buffer/this
 * @param src source buffer
 */
expublic void JNICALL Java_org_endurox_TypedUbf_Bconcat
  (JNIEnv * env, jobject data, jobject src)
{
    char *cdata_dst;
    long clen_dst;
    
    char *cdata_src;
    long clen_src;
    
    int ret = EXSUCCEED;
    
    /* get the context, switch */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
        return;
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata_dst, 
            &clen_dst))
    {
        UBF_LOG(log_error, "Failed to get dest/data buffer");
        EXFAIL_OUT(ret);
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata_src, 
            &clen_src))
    {
        UBF_LOG(log_error, "Failed to get source buffer");
        EXFAIL_OUT(ret);
    }
    
    /* Delete the field */
    if (EXSUCCEED!=Bconcat((UBFH *)cdata_dst, (UBFH *)cdata_src))
    {
        ndrxj_ubf_throw(env, Berror, "%s: Blen failed on %p buffer: %s", 
                __func__, cdata_dst, Bstrerror(Berror));
        EXFAIL_OUT(ret);
    }
    
out:
    
    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);

    return;
}

/**
 * Copy UBF data from source to dest
 * @param env Java env
 * @param data UBF data / this object / dest
 * @param src source UBF buffer
 */
expublic void JNICALL Java_org_endurox_TypedUbf_Bcpy
  (JNIEnv *env, jobject data, jobject src)
{
    char *cdata_dst;
    long clen_dst;
    
    char *cdata_src;
    long clen_src;
    
    int ret = EXSUCCEED;
    
    /* get the context, switch */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
        return;
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata_dst, 
            &clen_dst))
    {
        UBF_LOG(log_error, "Failed to get dest/data buffer");
        EXFAIL_OUT(ret);
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata_src, 
            &clen_src))
    {
        UBF_LOG(log_error, "Failed to get source buffer");
        EXFAIL_OUT(ret);
    }
    
    /* Delete the field */
    if (EXSUCCEED!=Bcpy((UBFH *)cdata_dst, (UBFH *)cdata_src))
    {
        ndrxj_ubf_throw(env, Berror, "%s: Bcpy failed on %p buffer: %s", 
                __func__, cdata_dst, Bstrerror(Berror));
        EXFAIL_OUT(ret);
    }
    
out:
    
    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);

    return;
}

/**
 * Copy only specified lists from source to dest. The dest buffer before
 * receiving data is reset to empty.
 * @param env java env
 * @param data dest buffer
 * @param src source buffer
 * @param bfldida array of fields
 */
expublic void JNICALL Java_org_endurox_TypedUbf_Bprojcpy
  (JNIEnv *env, jobject data, jobject src, jintArray bfldida)
{
    jboolean jarr_copy = JNI_FALSE;
    jint *jarr;
    int len;
    BFLDID *barra = NULL;
    int i;
    char *cdata_dst;
    long clen_dst;
    
    char *cdata_src;
    long clen_src;

    
    /* get the context, switch */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
       return; 
    }
    
    len = (*env)->GetArrayLength(env, bfldida);
    
    UBF_LOG(log_debug, "%s: Number of fields to copy: %d", __func__, len);
    
     barra = NDRX_MALLOC(sizeof(BFLDID)*(len+1));
    if (NULL==barra)
    {
        int err = errno;
        UBF_LOG(log_error, "%s: Failed to malloc %d bytes: %s", 
                __func__, (int)(sizeof(BFLDID)*(len+1)), strerror(err));
        userlog("%s: Failed to malloc %d bytes: %s", 
                __func__, (int)(sizeof(BFLDID)*(len+1)), strerror(err));
        ndrxj_ubf_throw(env, BEUNIX, "%s: Failed to malloc %d bytes: %s", 
                __func__, (int)(sizeof(BFLDID)*(len+1)), strerror(err));
        goto out;
    
    }
     
    jarr = (*env)->GetIntArrayElements(env, bfldida, &jarr_copy);
    
    for (i=0; i<len; i++)
    {
        barra[i] = (BFLDID)jarr[i];
    }
    
    barra[i] = BBADFLDID;
    
    if (EXSUCCEED!=Bprojcpy((UBFH*)cdata_dst, (UBFH*)cdata_src, barra))
    {
        UBF_LOG(log_error, "%s: Bprojcpy %p: %s", 
                __func__, barra, Bstrerror(Berror));
        ndrxj_ubf_throw(env, Berror, "%s: Bprojcpy %p: %s", 
                __func__, barra, Bstrerror(Berror));
        goto out;
    }

out:
    if (jarr_copy)
    {
        (*env)->ReleaseIntArrayElements(env, bfldida, jarr, JNI_ABORT);
    }

    if (NULL!=barra)
    {
        NDRX_FREE(barra);
    }

    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);
    return;

}


/**
 * Update target buffer from source buffer
 * @param env Java env
 * @param data UBF data / this object / dest
 * @param src source UBF buffer
 */
expublic void JNICALL Java_org_endurox_TypedUbf_Bupdate
  (JNIEnv * env, jobject data, jobject src)
{
    char *cdata_dst;
    long clen_dst;
    
    char *cdata_src;
    long clen_src;
    
    int ret = EXSUCCEED;
    
    /* get the context, switch */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
        return;
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata_dst, 
            &clen_dst))
    {
        UBF_LOG(log_error, "Failed to get dest/data buffer");
        EXFAIL_OUT(ret);
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata_src, 
            &clen_src))
    {
        UBF_LOG(log_error, "Failed to get source buffer");
        EXFAIL_OUT(ret);
    }
    
    /* Delete the field */
    if (EXSUCCEED!=Bupdate((UBFH *)cdata_dst, (UBFH *)cdata_src))
    {
        ndrxj_ubf_throw(env, Berror, "%s: Bcpy failed on %p buffer: %s", 
                __func__, cdata_dst, Bstrerror(Berror));
        EXFAIL_OUT(ret);
    }
    
out:
    
    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);

    return;
}

/**
 * Compare UBF buffer
 * @param env java env
 * @param data this UBF buffer/ub1
 * @param ub2 that UBF buffer
 * @return -1, 0, 1
 */
JNIEXPORT jint JNICALL Java_org_endurox_TypedUbf_Bcmp
  (JNIEnv *env, jobject data, jobject ub2)
{
    char *cdata;
    long clen;
    
    char *cdata_ub2;
    long clen_ub2;
    
    jint ret = EXSUCCEED;
    
    /* get the context, switch */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
        return ret;
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, 
            &clen))
    {
        UBF_LOG(log_error, "Failed to get ub1/data buffer");
        EXFAIL_OUT(ret);
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata_ub2, 
            &clen_ub2))
    {
        UBF_LOG(log_error, "Failed to get ub2 buffer");
        EXFAIL_OUT(ret);
    }
    
    /* Delete the field */
    ret=Bcmp((UBFH *)cdata, (UBFH *)cdata_ub2);
    
    if (EXSUCCEED!=Berror)
    {
        ndrxj_ubf_throw(env, Berror, "%s: Bcmp failed on %p vs %p: %s", 
                __func__, cdata, cdata_ub2, Bstrerror(Berror));
        EXFAIL_OUT(ret);
    }
    
out:
    
    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);

    return ret;
}

/* vim: set ts=4 sw=4 et cindent: */
