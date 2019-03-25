/**
 * @brief Queue access
 *
 * @file tpenqueue.c
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
 * Enqueue message
 * @param env java env
 * @param atmictx ATMI CTX
 * @param jqspace queue space
 * @param jqname queue name
 * @param jqctl queue control
 * @param idata input buffer (must not be NULL)
 * @param flags flags
 */
expublic void JNICALL Java_org_endurox_AtmiCtx_tpenqueue_int
  (JNIEnv * env, jobject atmiCtxObj, jstring jqspace, jstring jqname, jobject jqctl, 
        jobject idata, jlong flags)
{
    
    /* convert jqctl to C */
    TPQCTL q;
    jint ret = EXFAIL;
    TPCONTEXT_T ctx;
    /* set context */
    char *ibuf = NULL;
    long ilen = 0;
    char qspace[MAXTIDENT+1];
    char qname[TMQNAMELEN+1];
    
    /* get context & set */
    if (NULL==(ctx = ndrxj_get_ctx(env, atmiCtxObj, EXTRUE)))
    {
        goto out;
    }
    
    if (NULL==jqspace)
    {
        ndrxj_atmi_throw(env, idata, TPEINVAL, "Null argument: jqspace");
        EXFAIL_OUT(ret);
    }
    
    if (NULL==jqname)
    {
        ndrxj_atmi_throw(env, idata, TPEINVAL, "Null argument: jqname");
        EXFAIL_OUT(ret);
    }
    
    if (NULL==jqctl)
    {
        ndrxj_atmi_throw(env, idata, TPEINVAL, "Null argument: jqctl");
        EXFAIL_OUT(ret);
    }
    
    if (EXSUCCEED!=ndrxj_cvt_jstr_to_c(env, 
            atmiCtxObj, jqspace, qspace, sizeof(qspace)))
    {
        NDRX_LOG(log_error, "Failed to convert qspace to C");
        EXFAIL_OUT(ret);
    }
    
    if (EXSUCCEED!=ndrxj_cvt_jstr_to_c(env, 
            atmiCtxObj, jqname, qname, sizeof(qname)))
    {
        NDRX_LOG(log_error, "Failed to convert qname to C");
        EXFAIL_OUT(ret);
    }
    
    /* NOTE That will try NULL buffers too */
    
    /* get data buffer... */
    if (NULL!=idata)
    {
        if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, idata, &ibuf, &ilen, 
                NULL, EXFALSE, EXFALSE))
        {
            NDRX_LOG(log_error, "Failed to get data buffer!");
            goto out;
        }
    }
    
    /* convert qctl */
    if (EXSUCCEED!=ndrxj_atmi_TPQCTL_translate2c(env, atmiCtxObj, jqctl, &q))
    {
        NDRX_LOG(log_error, "ndrxj_atmi_TPQCTL_translate2c failed");
    }
    
    if (EXSUCCEED!=tpenqueue(qspace, qname, &q, ibuf, &ilen, flags))
    {        
        int err = tperrno;
        char errbuf[MAX_ERROR_LEN+1];
        
        NDRX_LOG(log_debug, "tpenqueue failed with %d", err);
        
        NDRX_STRCPY_SAFE(errbuf, tpstrerror(err));
        
        ndrxj_atmi_throw(env, idata, err, "%s", errbuf);
        goto out;
        
    }
    
    NDRX_LOG(log_debug, "tpenqueue to qspace[%s] queue [%s] OK", qspace, qname);
    
out:

    NDRX_LOG(log_debug, "returns %d", ret);    
    /* unset context */
    tpsetctxt(TPNULLCONTEXT, 0L);
    
    return ret;

}

/**
 * Enqueue message to disk
 * @param env java env
 * @param atmiCtxObj Atmi Context
 * @param jqspace queue space
 * @param jqname queue name
 * @param jqctl queue control struct
 * @param idata Input data
 * @param flags queue flags
 */
expublic void JNICALL Java_org_endurox_AtmiCtx_tpenqueue
  (JNIEnv * env, jobject atmiCtxObj, jstring jqspace, jstring jqname, jobject jqctl, 
        jobject idata, jlong flags)
{
    Java_org_endurox_AtmiCtx_tpenqueue_int(env, atmiCtxObj, jqspace, jqname, jqctl, 
        idata, flags);
}




/* vim: set ts=4 sw=4 et smartindent: */
