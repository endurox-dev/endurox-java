/**
 * @brief Compiled boolean expression ptr
 *
 * @file BExprTree.c
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
#define BEXPRTREE_CLASS "org/endurox/BExprTree"
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/

/**
 * Get expression tree pointer
 * @param env java env
 * @param ptrO pointer object
 * @return C pointer
 */
expublic char* ndrxj_BExprTree_ptr_get(JNIEnv *env, jobject ptrO)
{
    char *ret = NULL;
    jclass objClass = (*env)->GetObjectClass(env, ptrO);
    jfieldID ptr_fld;
    jlong jptr;
    
    if (NULL==objClass)
    {
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_ULOG, 
                "Failed to get object class: %s");
        goto out;
    }
    
    ptr_fld = (*env)->GetFieldID(env, objClass, "cPtr", "J");
    
    if (NULL==ptr_fld)
    {
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_ULOG, 
                "Failed to get cPtr_last_checked field from BFldLocInfo: %s");
        goto out;
    }
    
    jptr = (*env)->GetLongField(env, ptrO, ptr_fld);
    
    ret = (char *)jptr;
    
out:
    return ret;
}

/**
 * Store new pointer
 * @param env java env
 * @param ptrO java pointer object
 * @param new_ptr new pointer to store
 */
expublic void ndrxj_BExprTree_ptr_set(JNIEnv *env, jobject ptrO, char *new_ptr)
{
    jclass objClass = (*env)->GetObjectClass(env, ptrO);
    jfieldID ptr_fld;
    jlong jptr;
    
    if (NULL==objClass)
    {
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_ULOG, 
                "Failed to get object class: %s");
        goto out;
    }
    
    ptr_fld = (*env)->GetFieldID(env, objClass, "cPtr", "J");
    
    if (NULL==ptr_fld)
    {
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_ULOG, 
                "Failed to get cPtr_last_checked field from BExprTree: %s");
        goto out;
    }
    
    jptr = (long)new_ptr;
    
    (*env)->SetLongField(env, ptrO, ptr_fld, jptr);
    
    
out:
    return;
}

/**
 * Allocate new java object for storing the compiled boolean expression handler
 * This assumes that context is set
 * @param env java env
 * @param ctx ATMI Context that will be associated with the object.
 * @param ptr[in] ptr to save in java object
 * @return allocate java object or NULL (and exception is set)
 */
expublic jobject ndrxj_BExprTree_new(JNIEnv *env, char *ptr)
{
    jobject ret = NULL;
    jclass bclz;
    jmethodID mid;
    
    /* Set context if needed */
    
    NDRX_LOG(log_debug, "Allocating [%s]", BEXPRTREE_CLASS);
    
    bclz = (*env)->FindClass(env, BEXPRTREE_CLASS);
    
    if (NULL==bclz)
    {        
        NDRX_LOG(log_error, "Failed to find class [%s]", BEXPRTREE_CLASS);
        goto out;
    }
    
    /* create buffer object... */
    mid = (*env)->GetMethodID(env, bclz, "<init>", "(J)V");
    
    if (NULL==mid)
    {
        NDRX_LOG(log_error, "Cannot get buffer constructor!");
        goto out;
    }

    NDRX_LOG(log_debug, "About to NewObject(%s)", BEXPRTREE_CLASS);
    
    ret = (*env)->NewObject(env, bclz, mid, (jlong)ptr);
    
    if (NULL==ret)
    {
        NDRX_LOG(log_error, "Failed to create [%s]", BEXPRTREE_CLASS);
        goto out;
    }
    
    NDRX_LOG(log_debug, "NewObject() done");
    
out:
    return ret;
}

/**
 * Free up expression tree
 * @param env java env
 * @param exptree expression tree object onto which this method is called
 * @param cPtr C pointer
 */
expublic void JNICALL ndrxj_Java_org_endurox_BExprTree_Btreefree
  (JNIEnv * env, jobject exptree, jlong cPtr)
{
    /* switch contexts & perform free 
    
    if (NULL==(ndrxj_get_ctx(env, atmiCtxObj, EXTRUE)))
    {
        return;
    }
    */
    
    TPCONTEXT_T ctx;
    
    ctx = tpnewctxt(EXFALSE, EXTRUE);
    
    Btreefree((char *)cPtr);
    
    tpsetctxt(TPNULLCONTEXT, 0L);
    
    tpfreectxt(ctx);

}

/* vim: set ts=4 sw=4 et smartindent: */
