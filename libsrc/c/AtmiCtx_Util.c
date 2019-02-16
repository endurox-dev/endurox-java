/**
 * @brief Utility methods of Atmi Context class
 *
 * @file AtmiCtx_Util.c
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
#include <tpadm.h>
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/

/**
 * Get string version from the compiled filed id
 * @param env java env
 * @param atmiCtxObj ATMI Context object
 * @param bfldid compiled field id
 * @return 
 */
expublic jstring JNICALL Java_org_endurox_AtmiCtx_Btype
  (JNIEnv * env, jobject atmiCtxObj, jint bfldid)
{
    TPCONTEXT_T ctx;
    jstring ret = NULL;
    char *cret = NULL;
    
    if (NULL==(ctx = ndrxj_get_ctx(env, atmiCtxObj, EXTRUE)))
    {
        goto out;
    }
    
    cret = Btype((BFLDID)bfldid);
    
    if (NULL==cret)
    {
        /* throw exception */
        ndrxj_ubf_throw(env, Berror, "%s", Bstrerror(Berror));
        goto out;
    }
    
    /* create new UTF */
    ret=(jstring)((*env)->NewStringUTF(env, cret) );
    
out:
    tpsetctxt(TPNULLCONTEXT, 0L);
    return ret;
}

/**
 * Return field name 
 * @param env java env
 * @param atmiCtxObj ATMI Context object
 * @param bfldid compiled field id
 * @return field name instring
 */
expublic jstring JNICALL Java_org_endurox_AtmiCtx_Bfname
  (JNIEnv * env, jobject atmiCtxObj, jint bfldid)
{
    TPCONTEXT_T ctx;
    jstring ret = NULL;
    char *cret = NULL;
    
    if (NULL==(ctx = ndrxj_get_ctx(env, atmiCtxObj, EXTRUE)))
    {
        goto out;
    }
    
    cret = Bfname((BFLDID)bfldid);
    
    if (NULL==cret)
    {
        /* throw exception */
        ndrxj_ubf_throw(env, Berror, "%s", Bstrerror(Berror));
        goto out;
    }
    
    /* create new UTF */
    ret=(jstring)((*env)->NewStringUTF(env, cret) );
    
out:
    tpsetctxt(TPNULLCONTEXT, 0L);
    return ret;
}

/**
 * Get ATMI Context current timeout setting
 * @param env java env
 * @param atmiCtxObj atmi context object
 * @return timeout in secnods
 */
expublic JNIEXPORT jint JNICALL Java_org_endurox_AtmiCtx_tptoutget
  (JNIEnv * env, jobject atmiCtxObj)
{
    jint ret = EXFAIL;
    
    if (NULL== ndrxj_get_ctx(env, atmiCtxObj, EXTRUE))
    {
        goto out;
    }
    
    ret = (jint)tptoutget();
    
out:
    tpsetctxt(TPNULLCONTEXT, 0L);

    return ret;
}

/**
 * Set atmi context timeout setting
 * @param env java env
 * @param atmiCtxObj atmi context
 * @param tout timeout in seconds
 */
expublic JNIEXPORT void JNICALL Java_org_endurox_AtmiCtx_tptoutset
  (JNIEnv * env, jobject atmiCtxObj, jint tout)
{
    int ret = EXSUCCEED;
    
    if (NULL==ndrxj_get_ctx(env, atmiCtxObj, EXTRUE))
    {
        goto out;
    }
    
    if (EXSUCCEED!=tptoutset((int)tout))
    {
        ndrxj_atmi_throw(env, NULL, tperrno, "%s", tpstrerror(tperrno));
        goto out;
    }
    
out:
    tpsetctxt(TPNULLCONTEXT, 0L);
}

/**
 * Return list of ATMI buffers (a snapshoot of pointers)
 * @param env java env
 * @param data 
 * @return list of pointers
 */
expublic JNIEXPORT jlongArray JNICALL Java_org_endurox_AtmiCtx_getBuffers
  (JNIEnv * env, jobject atmiCtxObj)
{
    jlongArray ret = NULL;
    ndrx_growlist_t list;
    
    list.mem = NULL;
    
    if (NULL==ndrxj_get_ctx(env, atmiCtxObj, EXTRUE))
    {
        goto out;
    }
    
    if (EXSUCCEED!=ndrx_buffer_list(&list))
    {
        ndrxj_atmi_throw(env, NULL, TPESYSTEM, "Failed to get buffers list!");
        goto out;
    }
    
    NDRX_LOG(log_debug, "Nr allocated buffers: %d", list.maxindexused);

    /* Alloc java array */
    
    ret = (*env)->NewLongArray(env, list.maxindexused+1);
    
    if (NULL==ret)
    {
        ndrxj_atmi_throw(env, NULL, TPESYSTEM, "Failed to alloc java array!");
        goto out;
    }
    
    /* copy longs to java... */
    if (list.maxindexused > -1)
    {
        (*env)->SetLongArrayRegion(env, ret, 0, list.maxindexused+1, 
                (jlong *)list.mem);
    }
    
out:
    tpsetctxt(TPNULLCONTEXT, 0L);

    return ret;
}

/* vim: set ts=4 sw=4 et smartindent: */
