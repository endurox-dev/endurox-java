/**
 * @brief Perform call operations from the ATMI Context
 *
 * @file tpcallj.c
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
 * backend for tpcall
 * @param env java env
 * @param atmiCtxObj AtmiCtx object
 * @param svc service name
 * @param data ATMI Buffer reference
 * @param flags standard tpcall(3) flags
 */
JNIEXPORT jobject JNICALL Java_org_endurox_AtmiCtx_tpcall
  (JNIEnv *env, jobject atmiCtxObj, jstring svc, jobject idata, jlong flags)

{
    jobject ret = NULL;
    TPCONTEXT_T ctx;
    /* set context */
    char *ibuf = NULL;
    long ilen = 0;
    
    char *obuf = NULL;
    long olen = 0;
    
    jboolean n_svc_copy = EXFALSE;
    const char *n_svc = NULL;

    /* get context & set */
    
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
            goto out;
        }
    }
    
    /* Extract obuf, & olen from AtmiBufferRef */
    
    n_svc = (*env)->GetStringUTFChars(env, svc, &n_svc_copy);
    
    /* OK might get exception, but there could be buffer associated with it.. */
    if (EXSUCCEED!=tpcall((char *)n_svc, ibuf, ilen, &obuf, &olen, (long)flags))
    {
        int err = tperrno;
        jobject errdatabuf = NULL;
        char errbuf[MAX_ERROR_LEN+1];
        
        NDRX_LOG(log_debug, "Call failed with %d", err);
        /* save the error detail, and continue */
        
        /* if it is user error, return the data buffer */
        
        NDRX_STRCPY_SAFE(errbuf, tpstrerror(err));
        
        if (TPESVCFAIL==err)
        {
            errdatabuf = ndrxj_atmi_TypedBuffer_result_prep(env, atmiCtxObj, idata, ibuf, 
                ilen, obuf, olen);
            /* generate exception */
        }
        
        ndrxj_atmi_throw(env, errdatabuf, err, "%s", errbuf);
        goto out;
    }
    
    /* TODO: if class is changed of the buffer, create new object... 
     * and unset the pointer in the original AtmiBuf, so that it does not
     * make it free.
     */
    ret = ndrxj_atmi_TypedBuffer_result_prep(env, atmiCtxObj, idata, ibuf, 
        ilen, obuf, olen);
    
    /* Check exception, as data buffer can be NULL (in null response servers) */
    if ((*env)->ExceptionCheck(env)) 
    {
        NDRX_LOG(log_error, "Failed to prepare result of tpcall buffer");
        goto out;
    }
    
    NDRX_LOG(log_debug, "tpcall OK");
    
out:

    if (n_svc_copy)
    {
        (*env)->ReleaseStringUTFChars(env, svc, n_svc);
    }

    NDRX_LOG(log_debug, "%s returns %d", __func__, ret);
    
    /* unset context */
    tpsetctxt(TPNULLCONTEXT, 0L);
}

/* vim: set ts=4 sw=4 et smartindent: */
