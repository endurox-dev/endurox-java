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

/* vim: set ts=4 sw=4 et cindent: */
