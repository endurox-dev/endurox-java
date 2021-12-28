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
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/

/**
 * Return location field offset for fastadd operations
 * TODO: Add field IDs to cache?
 * @param env java env
 * @param loc location object from java
 * @param last_Baddfast ptr to where to store last field id checked
 * @return NULL in case of error, 
 */
expublic BFLDID* ndrxj_BFldLocInfo_ptr_get(JNIEnv *env, jobject loc, BFLDID *last_Baddfast)
{
    BFLDID *ret = NULL;
    jclass objClass /*= (*env)->GetObjectClass(env, loc)*/;
    jfieldID fid;
    jlong joffset;

    objClass = (*env)->FindClass(env, "org/endurox/BFldLocInfo");
    
    if (NULL==objClass)
    {
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_ULOG, 
                "Failed to get object class: %s");
        goto out;
    }
    
    fid = (*env)->GetFieldID(env, objClass, "cPtr_last_checked", "J");
    
    if (NULL==fid)
    {
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_ULOG, 
                "Failed to get cPtr_last_checked field from BFldLocInfo: %s");
        goto out;
    }
    
    joffset = (*env)->GetLongField(env, loc, fid);
    
    /* get last field id: */
    fid = (*env)->GetFieldID(env, objClass, "last_Baddfast", "I");
   
    if (NULL==fid)
    {
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_ULOG,
                "Failed to get last_Baddfast field from BFldLocInfo: %s");
        goto out;
    }

    /* Load results: */
    *last_Baddfast = (*env)->GetIntField(env, loc, fid);
    ret = (BFLDID *)(long)joffset;
    
out:
    return ret;
}

/**
 * Set field offset
 * @param env java env
 * @param loc java loc object
 * @param new_ptr new ptr to store
 * @param last_Baddfast last checked field id
 */
expublic void ndrxj_BFldLocInfo_ptr_set(JNIEnv *env, jobject loc, BFLDID *new_ptr, BFLDID last_Baddfast)
{
    jclass objClass = (*env)->GetObjectClass(env, loc);
    jfieldID fid;
    jlong joffset;
    jint jlast_Baddfast;
    
    if (NULL==objClass)
    {
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_ULOG, 
                "Failed to get object class: %s");
        goto out;
    }
    
    fid = (*env)->GetFieldID(env, objClass, "cPtr_last_checked", "J");
    
    if (NULL==fid)
    {
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_ULOG, 
                "Failed to get cPtr_last_checked field from BFldLocInfo: %s");
        goto out;
    }
    
    joffset = (long)new_ptr;
    
    (*env)->SetLongField(env, loc, fid, joffset);
    
    /* Load field id: */
    fid = (*env)->GetFieldID(env, objClass, "last_Baddfast", "I");

    if (NULL==fid)
    {
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_ULOG,
                "Failed to get cPtr_last_checked field from BFldLocInfo: %s");
        goto out;
    }

    jlast_Baddfast = (jint)last_Baddfast;

    (*env)->SetIntField(env, loc, fid, jlast_Baddfast);

    
out:
    return;
}


/* vim: set ts=4 sw=4 et smartindent: */
