/**
 * @brief XATMI Conversations implementation - Enduro/X mappings
 *
 * @file conversations.c
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
 * Perform tpconnect
 * @param env java env
 * @param atmiCtxObj ATMI Context
 * @param svc service name
 * @param idata input buffer
 * @param flags connect flags
 * @return connection descriptor
 */
expublic JNIEXPORT jint JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpconnect
  (JNIEnv * env, jobject atmiCtxObj, jstring svc, jobject idata, jlong flags)
{
    jint ret = EXFAIL;
    TPCONTEXT_T ctx;
    /* set context */
    char *ibuf = NULL;
    long ilen = 0;
    
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
        if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, idata, &ibuf, &ilen, 
                NULL, EXFALSE, EXFALSE))
        {
            NDRX_LOG(log_error, "Failed to get data buffer!");
            goto out;
        }
    }
    
    /* Extract obuf, & olen from AtmiBufferRef */
    n_svc = (*env)->GetStringUTFChars(env, svc, &n_svc_copy);
    
    /* OK might get exception, but there could be buffer associated with it.. */
    if (0>(ret=tpconnect((char *)n_svc, ibuf, ilen, (long)flags)))
    {
        int err = tperrno;
        char errbuf[MAX_ERROR_LEN+1];
        
        NDRX_LOG(log_debug, "Call failed with %d", err);
        /* save the error detail, and continue */
        
        /* if it is user error, return the data buffer */
        
        NDRX_STRCPY_SAFE(errbuf, tpstrerror(err));
        
        ndrxj_atmi_throw(env, idata, NULL, err, "%s", errbuf);
        goto out;
    }
    
    NDRX_LOG(log_debug, "tpconnect to [%s] OK cd=%d", n_svc, (int)ret);
    
out:

    if (n_svc_copy)
    {
        (*env)->ReleaseStringUTFChars(env, svc, n_svc);
    }

    NDRX_LOG(log_debug, "%s returns %d", __func__, ret);
    
    /* unset context */
    tpsetctxt(TPNULLCONTEXT, 0L);
    
    return ret;
}

/**
 * Receive data from conversation
 * @param env java environment
 * @param atmiCtxObj ATMI Context
 * @param cd call descriptor
 * @param idata data buffer
 * @param flags input flags
 * @return TprecvResult obj
 */
expublic JNIEXPORT jobject JNICALL ndrxj_Java_org_endurox_AtmiCtx_tprecv
  (JNIEnv * env, jobject atmiCtxObj, jint cd, jobject idata, jlong flags)
{
    jint ret = EXFAIL;
    jobject retObj = NULL;
    long revent = 0;
    TPCONTEXT_T ctx;
    int err;
    
    /* input buffer */
    char *ibuf = NULL;
    long ilen = 0;
    
    /* output buffer */
    char *obuf;
    long olen;
    
    jobject odata = NULL;
    char itype[XATMI_TYPE_LEN+1] = {EXEOS};
    char isubtype[XATMI_SUBTYPE_LEN+1]  = {EXEOS};
    
    /* get context & set */
    
    if (NULL==(ctx = ndrxj_get_ctx(env, atmiCtxObj, EXTRUE)))
    {
        goto out;
    } 
    
    /* get data buffer... */
    if (NULL!=idata)
    {
        if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, idata, &ibuf, &ilen, 
                NULL, EXFALSE, EXFALSE))
        {
            NDRX_LOG(log_error, "Failed to get data buffer!");
            goto out;
        }
        
        /* read buffer types... */
        if (EXFAIL==tptypes(ibuf, itype, isubtype))
        {
            NDRX_LOG(log_error, "Failed to get idata type infos: %s", 
                    tpstrerror(tperrno));
            ndrxj_atmi_throw(env, NULL, NULL, tperrno, "Failed to get odata type infos: %s", 
                    tpstrerror(tperrno));
            goto out;
        }
        
    }
    
    obuf = ibuf;
    olen = ilen;
    
    ret = tprecv((int)cd, &obuf, &olen, (long)flags, &revent);    
    err = tperrno;
   
    /* OK might get exception, but there could be buffer associated with it.. */
    if (EXSUCCEED!=ret && TPEEVENT!=err)
    {
        char errbuf[MAX_ERROR_LEN+1];
        jobject errdatabuf;
        NDRX_LOG(log_debug, "tprecv failed with %d", err);
        
        errdatabuf = idata;
            
        NDRX_STRCPY_SAFE(errbuf, tpstrerror(err));
        ndrxj_atmi_throw(env, errdatabuf, NULL, err, "%s", errbuf);
        goto out;
    }
    else
    {
        ret = EXSUCCEED;
    }
    
    NDRX_LOG(log_debug, "RECV OK cd=%d event %ld", (int)cd, revent);
    
    odata = ndrxj_atmi_TypedBuffer_result_prep(env, atmiCtxObj, 
                idata, ibuf, ilen, obuf, olen, itype, isubtype);
    
    if (NULL==(retObj = ndrxj_TprecvResult_new(env, atmiCtxObj, cd, odata, revent)))
    {
        NDRX_LOG(log_error, "Failed to generate reply object!");
        EXFAIL_OUT(ret);
    }
    
out:

    NDRX_LOG(log_debug, "returns %d", ret);
    
    /* unset context */
    tpsetctxt(TPNULLCONTEXT, 0L);
    
    if (EXSUCCEED==ret)
    {
        return retObj;
    }
    else
    {
        return NULL;
    }
}

/**
 * Send data to conversational end point
 * @param env java environment
 * @param atmiCtxObj ATMI Context
 * @param cd call descriptor
 * @param idata input data to be sent
 * @param flags ATMI Flags
 * @return ATMI Conv event or 0 (on OK)
 */
expublic JNIEXPORT jlong JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpsend
  (JNIEnv * env, jobject atmiCtxObj, jint cd, jobject idata, jlong flags)
{
    jint ret = EXFAIL;
    jobject retObj = NULL;
    long revent = 0;
    TPCONTEXT_T ctx;
    int err;
    
    /* input buffer */
    char *ibuf = NULL;
    long ilen = 0;
    
    /* get context & set */
    
    if (NULL==(ctx = ndrxj_get_ctx(env, atmiCtxObj, EXTRUE)))
    {
        goto out;
    } 
    
    /* get data buffer... */
    if (NULL!=idata)
    {
        if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, idata, &ibuf, &ilen, 
                NULL, EXFALSE, EXFALSE))
        {
            NDRX_LOG(log_error, "Failed to get data buffer!");
            goto out;
        }
    }
    
    ret = tpsend((int)cd, ibuf, ilen, (long)flags, &revent);    
    err = tperrno;
   
    /* OK might get exception, but there could be buffer associated with it.. */
    if (EXSUCCEED!=ret && TPEEVENT!=err)
    {
        char errbuf[MAX_ERROR_LEN+1];
        jobject errdatabuf;
        NDRX_LOG(log_debug, "tpsend failed with %d", err);
        
        errdatabuf = idata;
            
        NDRX_STRCPY_SAFE(errbuf, tpstrerror(err));
        
        ndrxj_atmi_throw(env, errdatabuf, NULL, err, "%s", errbuf);
        
        goto out;
    }
    
    NDRX_LOG(log_debug, "SEND OK cd=%d event %ld", (int)cd, revent);
    
out:

    NDRX_LOG(log_debug, "returns %ld", revent);
    
    /* unset context */
    tpsetctxt(TPNULLCONTEXT, 0L);
    
            
    return revent;
}

/**
 * Terminate ATMI conversation
 * @param env java env
 * @param atmiCtxObj ATMI Context object
 * @param cd call descriptor
 */
expublic JNIEXPORT void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpdiscon
  (JNIEnv * env, jobject atmiCtxObj, jint cd)
{
    jint ret = EXFAIL;
    TPCONTEXT_T ctx;
    int err;
    
    
    /* get context & set */    
    if (NULL==(ctx = ndrxj_get_ctx(env, atmiCtxObj, EXTRUE)))
    {
        goto out;
    } 
    
    ret = tpdiscon((int)cd);
   
    /* OK might get exception, but there could be buffer associated with it.. */
    if (EXSUCCEED!=ret)
    {
        char errbuf[MAX_ERROR_LEN+1];
        NDRX_LOG(log_debug, "tpdiscon failed with %d", err);
        
        NDRX_STRCPY_SAFE(errbuf, tpstrerror(err));
        
        ndrxj_atmi_throw(env, NULL, NULL, err, "%s", errbuf);
        
        goto out;
    }
    
    NDRX_LOG(log_debug, "DISCON OK cd=%d", (int)cd);
    
out:
    
    /* unset context */
    tpsetctxt(TPNULLCONTEXT, 0L);
    
            
    return;
}


/* vim: set ts=4 sw=4 et smartindent: */
