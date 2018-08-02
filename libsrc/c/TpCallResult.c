/**
 * @brief TpCall Result operations
 *
 * @file TpCallResult.c
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
#include <jni.h>
#include "org_endurox_TypedBuffer.h"
#include <atmi.h>
#include <oatmi.h>
#include <ndebug.h>
#include "libsrc.h"
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
#define TPCALLRESULT_CLASS  "org/endurox/TypedBuffer"
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/

/**
 * Set data object of the ATMI Buffer reference
 * @param env java env 
 * @param dataObj data object
 * @return Result object or 
 */
expublic jobject ndrxj_atmi_TpCallResult_new(JNIEnv *env, jobject ctx_obj,
        jobject dataObj, long tprucode)
{
    jobject ret = NULL;
    jclass bclz;
    jmethodID mid;
    
    /* Set context if needed */
    
    NDRX_LOG(log_debug, "Allocating TpCallResult...");
    
    bclz = (*env)->FindClass(env, TPCALLRESULT_CLASS);
    
    if (NULL==bclz)
    {        
        NDRX_LOG(log_error, "Failed to find class [%s]", TPCALLRESULT_CLASS);
        goto out;
    }
    
    /* create buffer object... */
    mid = (*env)->GetMethodID(env, bclz, "<init>", "(L" TPCALLRESULT_CLASS ";J)V");
    
    if (NULL==mid)
    {
        NDRX_LOG(log_error, "Cannot get %s constructor!", TPCALLRESULT_CLASS);
        goto out;
    }
    
    NDRX_LOG(log_debug, "About to NewObject() of TpCallResult");
    
    ret = (*env)->NewObject(env, bclz, mid, dataObj, (jlong)tprucode);
    
    if (NULL==ret)
    {
        NDRX_LOG(log_error, "Failed to create [%s] instance", TPCALLRESULT_CLASS);
        goto out;
    }

out:
    return ret;
}     
        
        
/* vim: set ts=4 sw=4 et cindent: */
