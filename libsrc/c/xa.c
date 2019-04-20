/**
 * @brief This is XA switch provider on top of the JAVA/JDBC API
 *  This will be activated from shared driver library.
 *  Static driver only.
 *
 * @file xa.c
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
#include "org_endurox_TypedBuffer.h"
#include <atmi.h>
#include <oatmi.h>
#include <ndebug.h>
#include <ondebug.h>
#include <oatmisrv_integra.h>
#include "libsrc.h"
#include <sys_unix.h>
#include "nerror.h"
#include <ndrstandard.h>
#include <xa.h>
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/


exprivate int xa_open_entry_stat(char *xa_info, int rmid, long flags);
exprivate int xa_close_entry_stat(char *xa_info, int rmid, long flags);
exprivate int xa_start_entry_stat(XID *xid, int rmid, long flags);
exprivate int xa_end_entry_stat(XID *xid, int rmid, long flags);
exprivate int xa_rollback_entry_stat(XID *xid, int rmid, long flags);
exprivate int xa_prepare_entry_stat(XID *xid, int rmid, long flags);
exprivate int xa_commit_entry_stat(XID *xid, int rmid, long flags);
exprivate int xa_recover_entry_stat(XID *xid, long count, int rmid, long flags);
exprivate int xa_forget_entry_stat(XID *xid, int rmid, long flags);
exprivate int xa_complete_entry_stat(int *handle, int *retval, int rmid, long flags);

exprivate int xa_open_entry(struct xa_switch_t *sw, char *xa_info, int rmid, long flags);
exprivate int xa_close_entry(struct xa_switch_t *sw, char *xa_info, int rmid, long flags);
exprivate int xa_start_entry(struct xa_switch_t *sw, XID *xid, int rmid, long flags);
exprivate int xa_end_entry(struct xa_switch_t *sw, XID *xid, int rmid, long flags);
exprivate int xa_rollback_entry(struct xa_switch_t *sw, XID *xid, int rmid, long flags);
exprivate int xa_prepare_entry(struct xa_switch_t *sw, XID *xid, int rmid, long flags);
exprivate int xa_commit_entry(struct xa_switch_t *sw, XID *xid, int rmid, long flags);
exprivate int xa_recover_entry(struct xa_switch_t *sw, XID *xid, long count, int rmid, long flags);
exprivate int xa_forget_entry(struct xa_switch_t *sw, XID *xid, int rmid, long flags);
exprivate int xa_complete_entry(struct xa_switch_t *sw, int *handle, int *retval, int rmid, long flags);

struct xa_switch_t ndrxdumssw = 
{ 
    .name = "ndrxjsw",
    .flags = TMNOFLAGS,
    .version = 0,
    .xa_open_entry = xa_open_entry_stat,
    .xa_close_entry = xa_close_entry_stat,
    .xa_start_entry = xa_start_entry_stat,
    .xa_end_entry = xa_end_entry_stat,
    .xa_rollback_entry = xa_rollback_entry_stat,
    .xa_prepare_entry = xa_prepare_entry_stat,
    .xa_commit_entry = xa_commit_entry_stat,
    .xa_recover_entry = xa_recover_entry_stat,
    .xa_forget_entry = xa_forget_entry_stat,
    .xa_complete_entry = xa_complete_entry_stat
};

/**
 * Perform init of the xa driver
 * this will parse the open string and will create XADataSource
 * the class name also needs to be set in the ini file.,
 * {"class":"org.postgresql.xa.PGXADataSource", "props":{"PROP":"VAL"}, "set":{"SetHost":"192.168.0.1", "SetPort":"7777"}}
 */
expublic int ndrxj_xa_init(void)
{
    int ret = EXSUCCEED;
    string_list_t *props = NULL;
    int nrprops =0;
    int nrsets = 0;
    string_list_t *sets = NULL;
    char clazz[PATH_MAX] = {EXEOS};
    char *opensrv = getenv(CONF_NDRX_XA_OPEN_STR);
    
    jobjectArray jsets = NULL;
    jstring jclazz = NULL;
    
    ndrx_ctx_priv_t ctxpriv;
    
    
    /* The JDBC driver shall library shall be added   */
    
    /* NDRX_XA_OPEN_STR -> json */
    
    if (EXSUCCEED!=ndrxj_xa_cfgparse(opensrv, &sets, &nrsets, clazz, sizeof(clazz)))
    {
        NDRX_LOG(log_error, "Failed to parse %s env", CONF_NDRX_XA_OPEN_STR);
        ret = TPEINVAL;
        goto out;
    }
    
    if (EXEOS==clazz[0])
    {
        NDRX_LOG(log_error, "Class name not specified!");
        ret = TPEINVAL;
        goto out;
    }
    
    /* build list of strings to send to Java loader... */
    
    ndrx_ctx_priv_get(&ctxpriv);
    
    if (NULL==(jsets = ndrxj_cvt_arr_c_to_java((JNIEnv *)ctxpriv.integptr1, 
        sets, nrsets)))
    {
        NDRX_LOG(log_error, "Failed to coverts sets to Java");
        ret = TPESYSTEM;
        goto out;
    }
    
    /* get the class string */
    jclazz = (*((JNIEnv *)ctxpriv.integptr1))->NewStringUTF((JNIEnv *)ctxpriv.integptr1, 
            clazz);
    
    if (NULL==jclazz)
    {
        NDRX_LOG(log_error, "Failed to coverts sets to Java");
        ret = TPESYSTEM;
        goto out;
    }
    
    /* lets initialize the class..., invoke the ATMI Context. 
     * so needs to find class, lets find method and then call it.
     */
    
    
    
out:
        
    /* TODO: Check exception and log it. */
    
    /* clear up references... */
    
    return ret;
}

/**
 * Open API.
 * This is called per thread. For java this will open an connection.
 * The connection handler must be kept within ATMI Context.
 * Active Java ATMI Context shall be stored in context data.
 * The open data shall be parsed with parson.
 * then configuration arrays as key:value props and sets must be loaded to java
 * The JSON could look like:
 * 
 * {"props":{"PROP":"VAL"}, "set":{"SetHost":"192.168.0.1", "SetPort":"7777"}}
 * 
 * This opens connection which in turn is stored in the ATMI Java Context.
 * 
 * @param switch 
 * @param xa_info
 * @param rmid
 * @param flags
 * @return 
 */
exprivate int xa_open_entry(struct xa_switch_t *sw, char *xa_info, int rmid, long flags)
{
    
    /* TODO: Do we need to check for existing open call?
     * the handler shall be present in java side..
     * So we need two string lists?
     */
             
    return XA_OK;
}

/**
 * Close entry
 * @param sw
 * @param xa_info
 * @param rmid
 * @param flags
 * @return 
 */
exprivate int xa_close_entry(struct xa_switch_t *sw, char *xa_info, int rmid, long flags)
{
    NDRX_LOG(log_error, "xa_close_entry() called");
    
    return XA_OK;
}

/**
 * Open text file in RMID folder. Create file by TXID.
 * Check for file existance. If start & not exists - ok .
 * If exists and join - ok. Otherwise fail.
 * @param xa_info
 * @param rmid
 * @param flags
 * @return 
 */
exprivate int xa_start_entry(struct xa_switch_t *sw, XID *xid, int rmid, long flags)
{
    return XA_OK;
}

/**
 * Terminate XA access
 * @param sw
 * @param xid
 * @param rmid
 * @param flags
 * @return 
 */
exprivate int xa_end_entry(struct xa_switch_t *sw, XID *xid, int rmid, long flags)
{
    
out:
    
    return XA_OK;
}

/**
 * Rollback
 * @param sw
 * @param xid
 * @param rmid
 * @param flags
 * @return 
 */
exprivate int xa_rollback_entry(struct xa_switch_t *sw, XID *xid, int rmid, long flags)
{
    return XA_OK;
}

/**
 * prepare
 * @param sw
 * @param xid
 * @param rmid
 * @param flags
 * @return 
 */
exprivate int xa_prepare_entry(struct xa_switch_t *sw, XID *xid, int rmid, long flags)
{
    
    return XA_OK;
}

/**
 * Commit
 * @param sw
 * @param xid
 * @param rmid
 * @param flags
 * @return 
 */
exprivate int xa_commit_entry(struct xa_switch_t *sw, XID *xid, int rmid, long flags)
{
    return XA_OK;
}

/**
 * Return list of trans
 * @param sw
 * @param xid
 * @param count
 * @param rmid
 * @param flags
 * @return 
 */
exprivate int xa_recover_entry(struct xa_switch_t *sw, XID *xid, long count, int rmid, long flags)
{
    return 0; /* 0 transactions found... */
}

/**
 * Forget tran
 * @param sw
 * @param xid
 * @param rmid
 * @param flags
 * @return 
 */
exprivate int xa_forget_entry(struct xa_switch_t *sw, XID *xid, int rmid, long flags)
{
    
    NDRX_LOG(log_error, "xa_forget_entry() - not implemented!!");
    return XA_OK;
}

/**
 * CURRENTLY NOT USED!!!
 * @param sw
 * @param handle
 * @param retval
 * @param rmid
 * @param flags
 * @return 
 */
exprivate int xa_complete_entry(struct xa_switch_t *sw, int *handle, int *retval, int rmid, long flags)
{
    
    NDRX_LOG(log_error, "xa_complete_entry() - not using!!");
    return XAER_RMERR;
}


/* Static entries */
exprivate int xa_open_entry_stat( char *xa_info, int rmid, long flags)
{
    return xa_open_entry(&ndrxdumssw, xa_info, rmid, flags);
}
exprivate int xa_close_entry_stat(char *xa_info, int rmid, long flags)
{
    return xa_close_entry(&ndrxdumssw, xa_info, rmid, flags);
}
exprivate int xa_start_entry_stat(XID *xid, int rmid, long flags)
{
    return xa_start_entry(&ndrxdumssw, xid, rmid, flags);
}

exprivate int xa_end_entry_stat(XID *xid, int rmid, long flags)
{
    return xa_end_entry(&ndrxdumssw, xid, rmid, flags);
}
exprivate int xa_rollback_entry_stat(XID *xid, int rmid, long flags)
{
    return xa_rollback_entry(&ndrxdumssw, xid, rmid, flags);
}
exprivate int xa_prepare_entry_stat(XID *xid, int rmid, long flags)
{
    return xa_prepare_entry(&ndrxdumssw, xid, rmid, flags);
}

exprivate int xa_commit_entry_stat(XID *xid, int rmid, long flags)
{
    return xa_commit_entry(&ndrxdumssw, xid, rmid, flags);
}

exprivate int xa_recover_entry_stat(XID *xid, long count, int rmid, long flags)
{
    return xa_recover_entry(&ndrxdumssw, xid, count, rmid, flags);
}
exprivate int xa_forget_entry_stat(XID *xid, int rmid, long flags)
{
    return xa_forget_entry(&ndrxdumssw, xid, rmid, flags);
}
exprivate int xa_complete_entry_stat(int *handle, int *retval, int rmid, long flags)
{
    return xa_complete_entry(&ndrxdumssw, handle, retval, rmid, flags);
}

/**
 * API entry of loading the driver
 * @param symbol
 * @param descr
 * @return XA switch or null
 */
struct xa_switch_t *ndrx_get_xa_switch(void)
{
    return &ndrxdumssw;
}


/* vim: set ts=4 sw=4 et smartindent: */
