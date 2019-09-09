/**
 * @brief Field location infos
 *
 * @file BFldLocInfo.c
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
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/

/* TODO: we need a getter function for current offset 
 * and we need a setter function to update the offset
 */

/**
 * Return location field offset for fastadd operations
 * @param env java env
 * @param loc location object from java
 * @return NULL in case of error, 
 */
expublic BFLDID* ndrxj_BFldLocInfo_ptr_get(JNIEnv *env, jobject loc)
{
    BFLDID *ret = NULL;
    jclass objClass /*= (*env)->GetObjectClass(env, loc)*/;
    jfieldID offset_fld;
    jlong joffset;

    objClass = (*env)->FindClass(env, "org/endurox/BFldLocInfo");
    
    if (NULL==objClass)
    {
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_ULOG, 
                "Failed to get object class: %s");
        goto out;
    }
    
    offset_fld = (*env)->GetFieldID(env, objClass, "cPtr_last_checked", "J");
    
    if (NULL==offset_fld)
    {
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_ULOG, 
                "Failed to get cPtr_last_checked field from BFldLocInfo: %s");
        goto out;
    }
    
    joffset = (*env)->GetLongField(env, loc, offset_fld);
    
    ret = (BFLDID *)joffset;
    
out:
    return ret;
}

/**
 * Set field offset
 * @param env java env
 * @param loc java loc object
 * @param new_ptr new ptr to store
 */
expublic void ndrxj_BFldLocInfo_ptr_set(JNIEnv *env, jobject loc, BFLDID *new_ptr)
{
    jclass objClass = (*env)->GetObjectClass(env, loc);
    jfieldID offset_fld;
    jlong joffset;
    
    if (NULL==objClass)
    {
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_ULOG, 
                "Failed to get object class: %s");
        goto out;
    }
    
    offset_fld = (*env)->GetFieldID(env, objClass, "cPtr_last_checked", "J");
    
    if (NULL==offset_fld)
    {
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_ULOG, 
                "Failed to get cPtr_last_checked field from BFldLocInfo: %s");
        goto out;
    }
    
    joffset = (long)new_ptr;
    
    (*env)->SetLongField(env, loc, offset_fld, joffset);
    
    
out:
    return;
}


/* vim: set ts=4 sw=4 et smartindent: */
