/**
 * @brief Compiled boolean expression ptr
 *
 * @file BNextResult.c
 */
/* -----------------------------------------------------------------------------
 * Enduro/X Middleware Platform for Distributed Transaction Processing
 * Copyright (C) 2009-2016, ATR Baltic, Ltd. All Rights Reserved.
 * Copyright (C) 2017-2019, Mavimax, Ltd. All Rights Reserved.
 * This software is released under one of the following licenses:
 * LGPL or Mavimax's license for commercial use.
 * See LICENSE text.
 *
 * C (as designed by Dennis Ritchie and later authors) language code is licensed
 * under Enduro/X Modified GNU Affero General Public License, version 3.
 * See LICENSE_C text.
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
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
#define BNEXTRESULT_CLASS "org/endurox/BNextResult"
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/

/**
 * Allocate iteration result
 * @param env java env
 * @param[in] bfldid  compiled filed id
 * @param[in] occ field occurrence
 * @param[in] len field length 
 * @return Result object
 */
expublic jobject ndrxj_BNextResult_new(JNIEnv *env, 
        BFLDID bfldid, BFLDOCC occ, BFLDLEN len)
{
    jobject ret = NULL;
    
    /* Set context if needed */
    
    UBF_LOG(log_debug, "Allocating [%s]", BNEXTRESULT_CLASS);
    
    NDRX_LOG(log_debug, "About to NewObject(%s)", BNEXTRESULT_CLASS);
    
    ret = (*env)->NewObject(env, ndrxj_clazz_BNextResult, ndrxj_clazz_BNextResult_mid_INIT, 
            (jint)bfldid, (jint)occ, (jint)len);
    
    if (NULL==ret)
    {
        NDRX_LOG(log_error, "Failed to create [%s]", BNEXTRESULT_CLASS);
        goto out;
    }
    
    NDRX_LOG(log_debug, "NewObject() done");
    
out:
    return ret;
}

/* vim: set ts=4 sw=4 et smartindent: */
