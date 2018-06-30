/**
 * @brief Perform call operations from the ATMI Context
 *
 * @file tpCall.c
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
#include "org_endurox_TpCallResult.h"
#include <atmi.h>
#include <oatmi.h>
#include <ndebug.h>
#include <ondebug.h>
#include <oatmisrv_integra.h>
#include "libsrc.h"
#include <sys_unix.h>
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/

/**
 * backend for tpcall
 * @param env java env
 * @param atmiCtxObj AtmiCtx object
 * @param svc service name
 * @param data ATMI Buffer reference
 * @param flags standard tpcall(3) flags
 */
JNIEXPORT jobject JNICALL Java_org_endurox_AtmiCtx_tpCall
  (JNIEnv *env, jobject atmiCtxObj, jstring svc, jobject idata, jlong flags)

{
    int ret = EXSUCCEED;
    TPCONTEXT_T ctx;
    /* set context */
    char *ibuf = NULL;
    long ilen = 0;
    
    char *obuf = NULL;
    long olen = 0;
    
    jboolean n_svc_copy = EXFALSE;
    const char *n_svc;

    /* TODO: get context & set */
    
    if (NULL==(ctx = ndrxj_get_ctx(env, atmiCtxObj, EXTRUE)))
    {
        goto out;
    } 
    
    /* get data buffer... */
    if (NULL!=idata)
    {
        if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, idata, &ibuf, &ilen))
        {
            NDRX_LOG(log_error, "Failed to get data buffer!");
            EXFAIL_OUT(ret);
        }
    }
    
    /* TODO: Extract obuf, & olen from AtmiBufferRef */
    
    (*env)->GetStringUTFChars(env, svc, &n_svc_copy);
    
    ret = tpcall((char *)n_svc, ibuf, ilen, &obuf, &olen, (long)flags);
    
    /* TODO: if class is changed of the buffer, create new object... 
     * and unset the pointer in the original AtmiBuf, so that it does not
     * make it free.
     */
    
out:

    if (n_svc_copy)
    {
        (*env)->ReleaseStringUTFChars(env, svc, n_svc);
    }

    NDRX_LOG(log_debug, "%s returns %d", __func__, ret);
    
    /* unset context */
    tpsetctxt(TPNULLCONTEXT, 0L);
}



/* vim: set ts=4 sw=4 et cindent: */
