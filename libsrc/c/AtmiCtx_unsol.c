/**
 * @brief Unsolicited message handling
 *
 * @file AtmiCtx_unsol.c
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

exprivate __thread JNIEnv* M_env;
exprivate __thread jobject M_atmiCtxObj;

/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/

/**
 * Set globals for unsolicated handling
 * @param env java env
 * @param atmiCtxObj Atmi context for the tpcall/tpgetrply/tpchkunsol callers
 */
expublic void ndrxj_atmictx_unsol_globals_set(JNIEnv* env, jobject atmiCtxObj)
{
    M_env = env;
    M_atmiCtxObj = atmiCtxObj;
}

/**
 * Unsolicited message dispatcher
 * @param data incoming data buffer
 * @param len data buffer len
 * @param flags ?
 */
exprivate void ndrx_unsol_dispatcher (char *data, long len, long flags) 
{
    /* here we will need a thread local java env handler and atmi context 
     * while performing C call from java 
     */
    
    /* Get in java env context  */
    
}

/**
 * Set unsolicited callback handler
 * @param env java env
 * @param atmiCtxObj atmi context object
 * @param jcb java object for unsol callback
 * @return curren unsol java hanlder
 */
JNIEXPORT void JNICALL Java_org_endurox_AtmiCtx_tpsetunsolC
  (JNIEnv * env, jobject atmiCtxObj, jobject jcb)
{
    TPCONTEXT_T ctx;
    int err;
    
    if (NULL==(ctx = ndrxj_get_ctx(env, atmiCtxObj, EXTRUE)))
    {
        return;
    }
    
    /* Register callback */    
    if (NULL!=jcb)
    {
        /* register java callback for unsol handler */
        if (TPUNSOLERR==tpsetunsol(ndrx_unsol_dispatcher))
        {
            err = tperrno;
            /* generate exception... */
            ndrxj_atmi_throw(env, NULL, err, tpstrerror(err));
            goto out;
        }
    }
    else 
    {
        if (TPUNSOLERR==tpsetunsol(NULL))
        {
            err = tperrno;
            /* generate exception... */
            ndrxj_atmi_throw(env, NULL, err, tpstrerror(err));
            goto out;
        }
    }
    
out:

    tpsetctxt(TPNULLCONTEXT, 0L);   
}

/* vim: set ts=4 sw=4 et smartindent: */
