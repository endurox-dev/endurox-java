/**
 * @brief Dequeue message from Q
 *
 * @file tpdequeue.c
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
#include <tpadm.h>
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/

/**
 * Dequeue message Enduro/X Q
 * @param env java env
 * @param atmiCtxObj Atmi Context
 * @param jqspace java queue space name
 * @param nodeid ... or cluster node id no which queue server is running
 * @param srvid and queue server id
 * @param jqname queue name
 * @param jqctl queue control
 * @param idata input data buffer into which to install output data (or reallocated
 *  this to new buffer returned)
 * @param flags dequeue flags
 * @return Dequeued data buffer object
 */
exprivate jobject tpdequeue_int (JNIEnv * env, jobject atmiCtxObj, jstring jqspace, 
        jshort nodeid, jshort srvid, jstring jqname, jobject jqctl, 
        jobject idata, jlong flags)
{
    /* convert jqctl to C */
    jobject retObj = NULL;
    TPQCTL q;
    int ret = EXSUCCEED;
    TPCONTEXT_T ctx;
    /* set context */
    char *ibuf = NULL;
    long ilen = 0;
    char qspace[MAXTIDENT+1];
    char qname[TMQNAMELEN+1];
    
    char itype[XATMI_TYPE_LEN+1] = "NULL";
    char isubtype[XATMI_SUBTYPE_LEN+1]  = {EXEOS};
    char *obuf;
    long olen;
    
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
    
    /* convert qctl */
    if (EXSUCCEED!=ndrxj_atmi_TPQCTL_translate2c(env, atmiCtxObj, jqctl, &q))
    {
        NDRX_LOG(log_error, "ndrxj_atmi_TPQCTL_translate2c failed");
        EXFAIL_OUT(ret);
    }
    
    /* get data buffer... */
    if (NULL!=idata)
    {
        if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, idata, &ibuf, &ilen, 
                NULL, EXFALSE, EXFALSE))
        {
            NDRX_LOG(log_error, "Failed to get data buffer!");
            goto out;
        }
        
        /* read buffer types... */
        if (EXFAIL==tptypes(ibuf, itype, isubtype))
        {
            NDRX_LOG(log_error, "Failed to get idata type infos: %s", 
                    tpstrerror(tperrno));
            ndrxj_atmi_throw(env, NULL, NULL, tperrno, "Failed to get odata type infos: %s", 
                    tpstrerror(tperrno));
            goto out;
        }

    }
    
    obuf = ibuf;
    olen = ilen;
    
    if (NULL!=jqspace)
    {
        NDRX_LOG(log_debug, "standard tpeequeue on [%s]/[%s]", qspace, qname);
        if (EXSUCCEED!=tpdequeue(qspace, qname, &q, &obuf, &olen, flags))
        {        
            int err = tperrno;
            char errbuf[MAX_ERROR_LEN+1];

            NDRX_LOG(log_debug, "tpenqueue failed with %d", err);

            NDRX_STRCPY_SAFE(errbuf, tpstrerror(err));

            ndrxj_atmi_throw(env, idata, jqctl, err, "%s", errbuf);
            goto errctl;

        }
    }
    else
    {
        NDRX_LOG(log_debug, "extended tpdequeue on [%hd]/[%hd]", 
                (short)nodeid, (short)srvid);
        if (EXSUCCEED!=tpdequeueex((short)nodeid, (short)srvid, qname, 
                &q, &obuf, &olen, flags))
        {        
            int err = tperrno;
            char errbuf[MAX_ERROR_LEN+1];

            NDRX_LOG(log_debug, "tpenqueue failed with %d", err);

            NDRX_STRCPY_SAFE(errbuf, tpstrerror(err));

            ndrxj_atmi_throw(env, idata, jqctl, err, "%s", errbuf);
            goto errctl;

        }
    }
        
    /* get reply buffer object */
    retObj = ndrxj_atmi_TypedBuffer_result_prep(env, atmiCtxObj, idata, ibuf, 
        ilen, obuf, olen, itype, isubtype);

errctl:
    /* restore the qctl */
    if (NULL==ndrxj_atmi_TPQCTL_translate2java(env, atmiCtxObj, jqctl, &q))
    {
        NDRX_LOG(log_error, "ndrxj_atmi_TPQCTL_translate2java failed");
        EXFAIL_OUT(ret);
    }

    
out:
        
        
    NDRX_LOG(log_debug, "%s returns %p", __func__, retObj);
    
    /* unset context */
    tpsetctxt(TPNULLCONTEXT, 0L);
    
    return retObj;
}

/**
 * Dequeue message
 * @param env java env
 * @param atmiCtxObj ATMI Context
 * @param jqspace queue space name
 * @param jqname queue name
 * @param jqctl queue control struct (will be updated)
 * @param idata input buffer may be-reused on return
 * @param flags flags
 * @return output buffer, if have one then idata is no longer valid
 */
expublic jobject JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpdequeue
  (JNIEnv * env, jobject atmiCtxObj, jstring jqspace, jstring jqname, 
        jobject jqctl, jobject idata, jlong flags)
{
    return tpdequeue_int (env, atmiCtxObj, jqspace, 
          EXFAIL, EXFAIL, jqname, jqctl, idata, flags);
}
/**
 * Dequeue message
 * @param env java env
 * @param atmiCtxObj ATMI Context
 * @param nodeid cluster node id
 * @param srvid server id (XATMI) on node
 * @param jqname queue name
 * @param jqctl queue control struct (will be updated)
 * @param idata input buffer may be-reused on return
 * @param flags flags
 * @return output buffer, if have one then idata is no longer valid
 */
expublic jobject JNICALL ndrxj_Java_org_endurox_AtmiCtx_tpdequeueex
  (JNIEnv * env, jobject atmiCtxObj, jshort nodeid, jshort srvid, 
        jstring jqname, jobject jqctl, jobject idata, jlong flags)
{
    return tpdequeue_int (env, atmiCtxObj, NULL, 
          nodeid, srvid, jqname, jqctl, idata, flags);
}

/* vim: set ts=4 sw=4 et smartindent: */
