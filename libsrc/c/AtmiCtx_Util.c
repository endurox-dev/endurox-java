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
    
    if (ret < 0)
    {
        /* throw exception */
        ndrxj_nstd_throw(env, Nerror, Nstrerror(Nerror));
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
    
    if (ret < 0)
    {
        /* throw exception */
        ndrxj_nstd_throw(env, Nerror, Nstrerror(Nerror));
    }
    
    /* create new UTF */
    ret=(jstring)((*env)->NewStringUTF(env, cret) );
    
out:
    tpsetctxt(TPNULLCONTEXT, 0L);
    return ret;
}


/* vim: set ts=4 sw=4 et cindent: */

