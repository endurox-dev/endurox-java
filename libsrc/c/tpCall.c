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
#include "org_endurox_AtmiBuf.h"
#include "org_endurox_AtmiBufRef.h"
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
 * @param ibuf input buffer
 * @param obuf output buffer
 * @param flags 
 */
JNIEXPORT void JNICALL Java_org_endurox_AtmiCtx_tpCall
  (JNIEnv *env, jobject atmiCtxObj, jstring svc, jobject idata, jobject odata, jlong flags)
{
    int ret = EXSUCCEED;
    /* set context */
    char *buf = NULL;
    long len = 0;
    jboolean n_svc_copy = EXFALSE;
    const char *n_svc;

    
    /* TODO: get context & set */
    tpsetctxt(M_srv_ctx, 0L);
    
    /* get data buffer... */
    if (NULL!=data)
    {
        if (EXSUCCEED!=ndrxj_atmi_AtmiBuf_get_buffer(env, data, &buf, &len))
        {
            NDRX_LOG(log_error, "Failed to get data buffer!");
            EXFAIL_OUT(ret);
        }
    }
    
    /*  */
    
    (*env)->GetStringUTFChars(env, svc, &n_svcname_copy);
    
    tpforward((char *)n_svcname, buf, len, (long)flags);
 
out:

    if (n_svcname_copy)
    {
        (*env)->ReleaseStringUTFChars(env, svcname, n_svcname);
    }

    NDRX_LOG(log_debug, "%s returns %d", __func__, ret);
    /* unset context */
    tpsetctxt(TPNULLCONTEXT, 0L);
}



/* vim: set ts=4 sw=4 et cindent: */
