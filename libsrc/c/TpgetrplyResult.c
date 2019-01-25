/**
 * @brief Backend code for 
 *
 * @file TpgetrplyResult.c
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
#define ALLOC_CLASS "org/endurox/TpgetrplyResult"
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/

/**
 * Allocate result buffer for tpgetrply()
 * @param[in] env java env
 * @param[in] ctx_obj Atmi context object
 * @param[in] cd call descriptor
 * @param[in] data XATMI data buffer ptr
 * @param[in] len XATMI data buffer len
 * @return Result object
 */
expublic jobject ndrxj_TpgetrplyResult_new(JNIEnv *env, jobject ctx_obj,
        int is_ctxset, int cd, char *data, long len)
{
    jobject ret = NULL;
    jclass bclz;
    jmethodID mid;
    jobject jdata = NULL;
    
    /* Set context if needed */
    
    UBF_LOG(log_debug, "Allocating [%s]", ALLOC_CLASS);
    
    bclz = (*env)->FindClass(env, ALLOC_CLASS);
    
    if (NULL==bclz)
    {        
        NDRX_LOG(log_error, "Failed to find class [%s]", ALLOC_CLASS);
        goto out;
    }
    
    /* create buffer object... */
    mid = (*env)->GetMethodID(env, bclz, "<init>", "(ILorg/endurox/TypedBuffer;)V");
    
    if (NULL==mid)
    {
        NDRX_LOG(log_error, "Cannot get buffer constructor!");
        goto out;
    }

    NDRX_LOG(log_debug, "About to NewObject(%s)", ALLOC_CLASS);

    jdata = ndrxj_atmi_TypedBuffer_translate(env, 
            ctx_obj, EXTRUE, data, len,
            NULL, NULL, EXTRUE);

    if (NULL==jdata)
    {
        NDRX_LOG(log_error, "Failed to translate to java buffer %p/%ld XATMI C buffer",
            data, len);
        goto out;
    }
    
    ret = (*env)->NewObject(env, bclz, mid, (jint)cd, jdata);
    
    if (NULL==ret)
    {
        NDRX_LOG(log_error, "Failed to create [%s]", ALLOC_CLASS);
        goto out;
    }
    
    NDRX_LOG(log_debug, "NewObject() done");
    
out:
    return ret;
}

/* vim: set ts=4 sw=4 et smartindent: */
