/**
 * @brief Delete operations on UBF buffer/backend.
 *
 * @file TypedUbf_Del.c
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
 * Delete field occurrence from UBF buffer
 * @param env java env
 * @param data UBF buffer
 * @param bfldid Compiled field id
 * @param occ field occurrence to delete
 */
expublic void JNICALL Java_org_endurox_TypedUbf_Bdel
  (JNIEnv *env, jobject data, jint bfldid, jint occ) 
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
        UBF_LOG(log_error, "Failed to get buffer data");
        goto out;
    }
    
    /* Delete the field */
    
    if (EXSUCCEED!=Bdel((UBFH*)cdata, (BFLDID)bfldid, (BFLDOCC)occ))
    {
        UBF_LOG(log_error, "%s: Bdel failed %d (%s): %s", 
                __func__, bfldid, Bfname(bfldid), Bstrerror(Berror));
        ndrxj_ubf_throw(env, Berror, "%s: Bdel failed %d (%s): %s", 
                __func__, bfldid, Bfname(bfldid), Bstrerror(Berror));
        goto out;
    }
    
out:
    
    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);
    
}

/**
 * Delete all field occurrences from UBF buffer
 * @param env java env
 * @param data UBF data buffer
 * @param bfldid compiled field id
 */
expublic void JNICALL Java_org_endurox_TypedUbf_Bdelall
  (JNIEnv * env, jobject data, jint bfldid)
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
        UBF_LOG(log_error, "Failed to get buffer data");
        goto out;
    }
    
    /* Delete the field */
    
    if (EXSUCCEED!=Bdelall((UBFH*)cdata, (BFLDID)bfldid))
    {
        UBF_LOG(log_error, "%s: Bdelall failed %d (%s): %s", 
                __func__, bfldid, Bfname(bfldid), Bstrerror(Berror));
        ndrxj_ubf_throw(env, Berror, "%s: Bdelall failed %d (%s): %s", 
                __func__, bfldid, Bfname(bfldid), Bstrerror(Berror));
        goto out;
    }
    
out:
    
    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);
}

/**
 * Delete list of fields from UBF buffer
 * @param env java env
 * @param data UBF buffer
 * @param bfldida array of fields to be erased with out terminating BBADFLDID.
 */
expublic void JNICALL Java_org_endurox_TypedUbf_Bdelete
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
            
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, &clen,
            NULL, EXFALSE, EXFALSE))
    {
        UBF_LOG(log_error, "Failed to get buffer data");
        goto out;
    }
    
    /* Delete the field */
    
    if (EXSUCCEED!=Bdelete((UBFH*)cdata, barra))
    {
        UBF_LOG(log_error, "%s: Bdelete %p: %s", 
                __func__, barra, Bstrerror(Berror));
        ndrxj_ubf_throw(env, Berror, "%s: Bdelete %p: %s", 
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

/* vim: set ts=4 sw=4 et smartindent: */
