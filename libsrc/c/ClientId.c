/**
 * @brief CLIENTID/ClientId type translations
 *
 * @file ClientId.c
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
#include <atmi.h>
#include <oatmi.h>
#include <ndebug.h>
#include <ondebug.h>
#include <oatmisrv_integra.h>
#include "libsrc.h"
#include <sys_unix.h>
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/

#define CLIENTID_CLASS  "org/endurox/ClientId"
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/

/**
 * Translate java client id to C client id
 * @param env java env
 * @param in_jcltid java client id
 * @param out_cltid c client id
 * @return EXSUCCEED/EXFAIL
 */
expublic int ndrxj_atmi_ClientId_translate_toc(JNIEnv *env, 
        jobject in_jcltid, CLIENTID *out_cltid)
{
    
}

/**
 * Translate CLIENTID to java ClientID object.
 * 
 * CONTEXT: Assume that it is already set.
 * 
 * @param env java environment
 * @param atmictx ATMI Context object
 * @param is_ctxset is context set?
 * @param cltid C client ID which needs to be translated to 
 * @return ClientId Object or NULL
 */
expublic jobject ndrxj_atmi_ClientId_translate(JNIEnv *env, 
            jobject ctx_obj, int is_ctxset, CLIENTID *cltid)
{
    jobject ret = NULL;
    jclass bclz;
    jmethodID mid;
    jstring jclientdata;
    int we_set_ctx = EXFALSE;

    /* Set context if needed */
    if (!is_ctxset)
    {
        if (NULL==ndrxj_get_ctx(env, ctx_obj, EXTRUE))
        {
            goto out;
        }
        
        we_set_ctx = EXTRUE;
    }
    
    NDRX_LOG(log_debug, "Allocating ClientID...");
    
    bclz = (*env)->FindClass(env, CLIENTID_CLASS);
    
    if (NULL==bclz)
    {        
        NDRX_LOG(log_error, "Failed to find class [%s]", CLIENTID_CLASS);
        goto out;
    }
    
    /* create buffer object... */
    mid = (*env)->GetMethodID(env, bclz, "<init>", "(Ljava/lang/String;)V");
    
    if (NULL==mid)
    {
        NDRX_LOG(log_error, "Cannot get %s constructor!", CLIENTID_CLASS);
        goto out;
    }
    
    /* get the string of client id */
    
    jclientdata = (*env)->NewStringUTF(env, cltid->clientdata);
    
    NDRX_LOG(log_debug, "About to NewObject() of ClientID");
    
    ret = (*env)->NewObject(env, bclz, mid, jclientdata);
    
    (*env)->DeleteLocalRef( env, bclz);
    
    if (NULL==ret)
    {
        NDRX_LOG(log_error, "Failed to create [%s] instance", CLIENTID_CLASS);
        goto out;
    }
    
    NDRX_LOG(log_debug, "NewObject() done of ClientID");
    
    /* delete local reference to avoid the leak */
    
    (*env)->DeleteLocalRef(env, jclientdata);
    
out:
    
    if (we_set_ctx)
    {
        /* return back to NULL */
        tpsetctxt(TPNULLCONTEXT, 0L);
    }

    return ret;
}

/* vim: set ts=4 sw=4 et smartindent: */
