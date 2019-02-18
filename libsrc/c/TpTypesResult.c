/**
 * @brief Allocate tptypes() return object
 *
 * @file TpTypesResult.c
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
#define ALLOC_CLASS "org/endurox/TpTypesResult"
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/

/**
 * Allocate buffer type result
 * @param env java env
 * @param btype buffer type
 * @param sub_type sub-type (if not set -> emtpy string)
 * @param size allocated buffer size
 * @return allocated object
 */
expublic jobject ndrxj_TpTypesResult_new(JNIEnv *env, 
        char *btype, char *sub_type, long size)
{
    jobject ret = NULL;
    jclass clz;
    jmethodID mid;
    jstring jtype;
    jstring jsub_type;
    /* have two strings  */
    
    /* Set context if needed */
    
    UBF_LOG(log_debug, "Allocating [%s]", ALLOC_CLASS);
    
    clz = (*env)->FindClass(env, ALLOC_CLASS);
    
    if (NULL==clz)
    {        
        NDRX_LOG(log_error, "Failed to find class [%s]", ALLOC_CLASS);
        goto out;
    }
    
    /* create buffer object... */
    mid = (*env)->GetMethodID(env, clz, "<init>", "(Ljava/lang/String;Ljava/lang/String;J)V");
    
    if (NULL==mid)
    {
        NDRX_LOG(log_error, "Cannot get [%s] constructor!", ALLOC_CLASS);
        goto out;
    }
    
    jtype = (*env)->NewStringUTF(env, btype);
    jsub_type = (*env)->NewStringUTF(env, sub_type);

    /* TODO: How about exception checking right here? */
    NDRX_LOG(log_debug, "About to NewObject(%s)", ALLOC_CLASS);
    
    ret = (*env)->NewObject(env, clz, mid, jtype, jsub_type, (jlong)size);
    
    if (NULL==ret)
    {
        NDRX_LOG(log_error, "Failed to create [%s]", ALLOC_CLASS);
        goto out;
    }
    
    NDRX_LOG(log_debug, "NewObject() done");
    
out:
    
    (*env)->DeleteLocalRef(env, clz);
    (*env)->DeleteLocalRef(env, jtype);
    (*env)->DeleteLocalRef(env, jsub_type);
    
    return ret;
}

/* vim: set ts=4 sw=4 et smartindent: */
