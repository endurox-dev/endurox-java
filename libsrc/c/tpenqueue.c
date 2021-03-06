/**
 * @brief Queue access
 *
 * @file tpenqueue.c
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
 * @param nodeid Cluster ID, -1 if not used
 * @param srvid server id on cluster, -1 if not used
 * @param jqname queue name
 * @param jqctl queue control
 * @param idata input buffer (must not be NULL)
 * @param flags flags
 */
exprivate void tpenqueue_int
  (JNIEnv * env, jobject atmiCtxObj, jstring jqspace, 
        jshort nodeid, jshort srvid, jstring jqname, jobject jqctl, 
        jobject idata, jlong flags)
{
    /* convert jqctl to C */
    TPQCTL q;
    int ret = EXSUCCEED;
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
    
    if (EXFAIL==nodeid && EXFAIL==srvid && NULL==jqspace)
    {
        ndrxj_atmi_throw(env, idata, NULL, TPEINVAL, "Null argument: jqspace");
        EXFAIL_OUT(ret);
    }
    
    if (NULL==jqname)
    {
        ndrxj_atmi_throw(env, idata, NULL, TPEINVAL, "Null argument: jqname");
        EXFAIL_OUT(ret);
    }
    
    if (NULL==jqctl)
    {
        ndrxj_atmi_throw(env, idata, NULL, TPEINVAL, "Null argument: jqctl");
        EXFAIL_OUT(ret);
    }
    
    if (NULL!=jqspace && EXSUCCEED!=ndrxj_cvt_jstr_to_c(env, 
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
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, idata, &ibuf, &ilen, 
            NULL, EXFALSE, EXFALSE))
    {
        NDRX_LOG(log_error, "Failed to get data buffer!");
        goto out;
    }
    
    /* convert qctl */
    if (EXSUCCEED!=ndrxj_atmi_TPQCTL_translate2c(env, atmiCtxObj, jqctl, &q, idata))
    {
        NDRX_LOG(log_error, "ndrxj_atmi_TPQCTL_translate2c failed");
        EXFAIL_OUT(ret);
    }
    
    if (NULL!=jqspace)
    {
        NDRX_LOG(log_debug, "standard tpenqueue on [%s]/[%s]", qspace, qname);
        if (EXSUCCEED!=tpenqueue(qspace, qname, &q, ibuf, ilen, flags))
        {        
            int err = tperrno;
            char errbuf[MAX_ERROR_LEN+1];

            NDRX_LOG(log_debug, "tpenqueue failed with %d", err);

            NDRX_STRCPY_SAFE(errbuf, tpstrerror(err));

            ndrxj_atmi_throw(env, idata, jqctl, err, "%s", errbuf);

        }
    }
    else
    {
        NDRX_LOG(log_debug, "extended tpenqueue on [%hd]/[%hd]", 
                (short)nodeid, (short)srvid);
        if (EXSUCCEED!=tpenqueueex((short)nodeid, (short)srvid, qname, 
                &q, ibuf, ilen, flags))
        {        
            int err = tperrno;
            char errbuf[MAX_ERROR_LEN+1];

            NDRX_LOG(log_debug, "tpenqueue failed with %d", err);

            NDRX_STRCPY_SAFE(errbuf, tpstrerror(err));

            ndrxj_atmi_throw(env, idata, jqctl, err, "%s", errbuf);

        }
    }
    
    /* restore the qctl */
    if (NULL==ndrxj_atmi_TPQCTL_translate2java(env, atmiCtxObj, jqctl, &q))
    {
        NDRX_LOG(log_error, "ndrxj_atmi_TPQCTL_translate2java failed");
        EXFAIL_OUT(ret);
    }
    
    /* NDRX_LOG(log_debug, "tpenqueue to qspace[%s] queue [%s] OK", qspace, qname); */
    
out:

    NDRX_LOG(log_debug, "returns %d", ret);    
    /* unset context */
    tpsetctxt(TPNULLCONTEXT, 0L);

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
expublic void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpenqueue
  (JNIEnv * env, jobject atmiCtxObj, jstring jqspace, jstring jqname, jobject jqctl, 
        jobject idata, jlong flags)
{
    tpenqueue_int(env, atmiCtxObj, jqspace, EXFAIL, EXFAIL, jqname, jqctl, 
        idata, flags);
}

/**
 * Enqueue message, extended version
 * @param env java env
 * @param atmiCtxObj Atmi Context
 * @param nodeid nodeid
 * @param srvid server id
 * @param qname queue name
 * @param ctl queue control struct
 * @param idata input data
 * @param flags queue flags
 */
JNIEXPORT void JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpenqueueex
  (JNIEnv * env, jobject atmiCtxObj, jshort nodeid, jshort srvid, jstring jqname, 
        jobject jqctl, jobject idata, jlong flags)
{
    tpenqueue_int(env, atmiCtxObj, NULL, nodeid, srvid, jqname, jqctl, 
        idata, flags);
}

/* vim: set ts=4 sw=4 et smartindent: */
