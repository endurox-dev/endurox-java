/**
 * @brief Compiled boolean expression ptr
 *
 * @file BExprTree.c
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
    jlong jptr;
    
    jptr = (*env)->GetLongField(env, ptrO, ndrxj_clazz_BExprTree_fid_cPtr);
    
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
    jlong jptr;
        
    jptr = (long)new_ptr;
    
    (*env)->SetLongField(env, ptrO, ndrxj_clazz_BExprTree_fid_cPtr, jptr);
    
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
    
    NDRX_LOG(log_debug, "About to NewObject(%s)", BEXPRTREE_CLASS);
    
    ret = (*env)->NewObject(env, ndrxj_clazz_BExprTree, 
            ndrxj_clazz_BExprTree_mid_INIT, (jlong)ptr);
    
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
