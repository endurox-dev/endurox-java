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

struct xa_switch_t ndrxjsw = 
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
    int nrsets = 0;
    string_list_t *sets = NULL;
    char clazz[PATH_MAX] = {EXEOS};
    char *opensrv = getenv(CONF_NDRX_XA_OPEN_STR);
    jobjectArray jsets = NULL;
    jstring jclazz = NULL;
    ndrx_ctx_priv_t *ctxpriv;
    jmethodID mid;
    
    jclass ctxClass = NULL;
    
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
    
    ctxpriv = ndrx_ctx_priv_get();
    
    if (NULL==(jsets = ndrxj_cvt_arr_c_to_java((JNIEnv *)ctxpriv->integptr1, 
        sets, nrsets)))
    {
        NDRX_LOG(log_error, "Failed to coverts sets to Java");
        ret = TPESYSTEM;
        goto out;
    }
    
    /* get the class string */
    jclazz = (*((JNIEnv *)ctxpriv->integptr1))->NewStringUTF((JNIEnv *)ctxpriv->integptr1, 
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
    
    /* Get the class for the context object */
    ctxClass = (*(JNIEnv *)ctxpriv->integptr1)->GetObjectClass((JNIEnv *)ctxpriv->integptr1, 
        (jobject)ctxpriv->integptr2);
    
    if (NULL==ctxClass)
    {
        NDRX_LOG(log_error, "Failed to get ctx object class");
        ret = TPESYSTEM;
        goto out;
    }
    
    /* Get the method id  */
    
    mid = (*(JNIEnv *)ctxpriv->integptr1)->GetMethodID((JNIEnv *)ctxpriv->integptr1, 
        ctxClass, "initXADataSource",
        "(Ljava/lang/String;[Ljava/lang/String;)I");
    
    if (NULL==mid)
    {
        NDRX_LOG(log_error, "Failed to get initXADataSource() method!");
        ret = TPESYSTEM;
        goto out;
    }
    
    /* Call the method */
    ret = (*(JNIEnv *)ctxpriv->integptr1)->CallIntMethod((JNIEnv *)ctxpriv->integptr1, 
        (jobject)ctxpriv->integptr2, mid, jclazz, jsets);
    
    if (EXSUCCEED!=ret)
    {
        NDRX_LOG(log_error, "Failed to create XA Data Source in Java side: %d",
                tpstrerror(ret));
        ret = TPESYSTEM;
        goto out;
    }
    
    NDRX_LOG(log_info, "XA Data Source [%s] registered OK", clazz);
    
out:
    
    /* Check exception and log it. */
    if ((*(JNIEnv *)ctxpriv->integptr1)->ExceptionCheck((JNIEnv *)ctxpriv->integptr1))
    {
        NDRXJ_LOG_EXCEPTION(((JNIEnv *)ctxpriv->integptr1), log_error, NDRXJ_LOGEX_ULOG, 
                "Failed to active XA Data Source class [%s]: %s", clazz);
    }

    /* clear up references... */
    if (NULL!=ctxClass)
    {
        (*(JNIEnv *)ctxpriv->integptr1)->DeleteLocalRef((JNIEnv *)ctxpriv->integptr1, 
                ctxClass);
    }

    if (NULL!=jsets)
    {
        (*(JNIEnv *)ctxpriv->integptr1)->DeleteLocalRef((JNIEnv *)ctxpriv->integptr1, 
                jsets);
    }

    if (NULL!=jclazz)
    {
        (*(JNIEnv *)ctxpriv->integptr1)->DeleteLocalRef((JNIEnv *)ctxpriv->integptr1, 
                jclazz);
    }
    
    return ret;
}

/**
 * Perform info call
 * @param func function name
 * @param sw XA Switch current
 * @param xa_info XA Info string
 * @param rmid rmid
 * @param flags flags
 * @return error code, XA
 */
exprivate int xa_info_entry(char *func, struct xa_switch_t *sw, char *xa_info, int rmid, long flags)
{
   /* TODO: Do we need to check for existing open call?
     * the handler shall be present in java side..
     * So we need two string lists?
     */
    ndrx_ctx_priv_t *ctxpriv;
    jmethodID mid;
    jclass ctxClass = NULL;
    int ret = XA_OK;
    
    ctxpriv = ndrx_ctx_priv_get();
    
    /* Get the class for the context object */
    ctxClass = (*(JNIEnv *)ctxpriv->integptr1)->GetObjectClass((JNIEnv *)ctxpriv->integptr1, 
        (jobject)ctxpriv->integptr2);
    
    if (NULL==ctxClass)
    {
        NDRX_LOG(log_error, "Failed to get ctx object class");
        ret = XAER_RMERR;
        goto out;
    }
    
    /* Get the method id  */
    mid = (*(JNIEnv *)ctxpriv->integptr1)->GetMethodID((JNIEnv *)ctxpriv->integptr1, 
        ctxClass, func,
        "(J)I");
    
    if (NULL==mid)
    {
        NDRX_LOG(log_error, "Failed to get %s() method!", func);
        ret = XAER_RMERR;
        goto out;
    }
    
    ret = (*(JNIEnv *)ctxpriv->integptr1)->CallIntMethod((JNIEnv *)ctxpriv->integptr1, 
        (jobject)ctxpriv->integptr2, mid, (jlong)flags);
    
    NDRX_LOG(log_debug, "Java %s returns %d", func, ret);
    
out:
    
    if ((*(JNIEnv *)ctxpriv->integptr1)->ExceptionCheck((JNIEnv *)ctxpriv->integptr1))
    {
        NDRXJ_LOG_EXCEPTION(((JNIEnv *)ctxpriv->integptr1), log_error, NDRXJ_LOGEX_ULOG, 
                "% failed: %s", func);
        if (XA_OK==ret)
        {
            ret = XAER_RMERR;
        }
        (*(JNIEnv *)ctxpriv->integptr1)-> ExceptionClear((JNIEnv *)ctxpriv->integptr1);
    }

    /* clear up references... */
    if (NULL!=ctxClass)
    {
        (*(JNIEnv *)ctxpriv->integptr1)->DeleteLocalRef((JNIEnv *)ctxpriv->integptr1, 
                ctxClass);
    }
    
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
    return xa_info_entry("xa_open_entry",  sw, xa_info, rmid, flags);
}

/**
 * Close entry.
 * This will close connections at java side only.
 * @param sw xa switch
 * @param xa_info close string
 * @param rmid RM id
 * @param flags flags
 * @return xa err
 */
exprivate int xa_close_entry(struct xa_switch_t *sw, char *xa_info, int rmid, long flags)
{
    return xa_info_entry("xa_close_entry",  sw, xa_info, rmid, flags);
}


/**
 * Call the java with XID related operation
 * @param func function name
 * @param sw xa switch 
 * @param xid xid for trx
 * @param rmid resource manager id
 * @param flags flags
 * @return xa err
 */
exprivate int xa_xid_entry(char *func, struct xa_switch_t *sw, XID *xid, int rmid, long flags)
{
    ndrx_ctx_priv_t *ctxpriv;
    jmethodID mid;
    jclass ctxClass = NULL;
    int ret = XA_OK;
    jobject jxid = NULL;
    
    ctxpriv = ndrx_ctx_priv_get();
    
    /* create xid first */
    jxid = ndrxj_cvt_xid_to_java((JNIEnv *)ctxpriv->integptr1, xid);
    
    if (NULL==xid)
    {
        NDRX_LOG(log_error, "Failed to convert C xid to Java");
        ret = XAER_RMERR;
        goto out;
    }
    
    /* Get the class for the context object */
    ctxClass = (*(JNIEnv *)ctxpriv->integptr1)->GetObjectClass((JNIEnv *)ctxpriv->integptr1, 
        (jobject)ctxpriv->integptr2);
    
    if (NULL==ctxClass)
    {
        NDRX_LOG(log_error, "Failed to get ctx object class");
        ret = XAER_RMERR;
        goto out;
    }
    
    /* Get the method id  */
    mid = (*(JNIEnv *)ctxpriv->integptr1)->GetMethodID((JNIEnv *)ctxpriv->integptr1, 
        ctxClass, func,
        "(Ljavax/transaction/xa/Xid;J)I");
    
    if (NULL==mid)
    {
        NDRX_LOG(log_error, "Failed to get %s() method!", func);
        ret = XAER_RMERR;
        goto out;
    }
    
    ret = (*(JNIEnv *)ctxpriv->integptr1)->CallIntMethod((JNIEnv *)ctxpriv->integptr1, 
        (jobject)ctxpriv->integptr2, mid, jxid, (jlong)flags);
    
    NDRX_LOG(log_debug, "Java %s returns %d", func, ret);
    
out:
    
    if ((*(JNIEnv *)ctxpriv->integptr1)->ExceptionCheck((JNIEnv *)ctxpriv->integptr1))
    {
        NDRXJ_LOG_EXCEPTION(((JNIEnv *)ctxpriv->integptr1), log_error, NDRXJ_LOGEX_ULOG, 
                "% failed: %s", func);
        if (XA_OK==ret)
        {
            ret = XAER_RMERR;
        }
        (*(JNIEnv *)ctxpriv->integptr1)-> ExceptionClear((JNIEnv *)ctxpriv->integptr1);
    }

    /* clear up references... */
    if (NULL!=ctxClass)
    {
        (*(JNIEnv *)ctxpriv->integptr1)->DeleteLocalRef((JNIEnv *)ctxpriv->integptr1, 
                ctxClass);
    }
    
    return ret;
}


/**
 * In this case we need to convert our internal XID format to java XID
 * and pass it in the call.
 * @param xa_info
 * @param rmid
 * @param flags
 * @return 
 */
exprivate int xa_start_entry(struct xa_switch_t *sw, XID *xid, int rmid, long flags)
{
    return xa_xid_entry("xa_start_entry", sw, xid, rmid, flags);
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
    return xa_xid_entry("xa_end_entry", sw, xid, rmid, flags);
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
    return xa_xid_entry("xa_rollback_entry", sw, xid, rmid, flags);
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
    return xa_xid_entry("xa_prepare_entry", sw, xid, rmid, flags);
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
    return xa_xid_entry("xa_commit_entry", sw, xid, rmid, flags);
}

/**
 * Return list of trans.
 * Java returns full list, but our buffer is limited.
 * Thus we load the `count' number of items, but we will return the actual
 * number of java items
 * @param sw xa switch
 * @param xid buffer where to load xids
 * @param count number of xid buffer size
 * @param rmid resourcemanager id
 * @param flags flags
 * @return XA_OK, XERR
 */
exprivate int xa_recover_entry(struct xa_switch_t *sw, XID *xid, long count, int rmid, long flags)
{
    ndrx_ctx_priv_t *ctxpriv;
    jmethodID mid;
    jclass ctxClass = NULL;
    jclass xidClass = NULL;
    
    jfieldID fxidarr; /* Array of xids */
    jfieldID fret;  /* return code */
            
    jobject retObj = NULL;
    int ret = XA_OK;
    jarray xarr = NULL;
    int jcount;
    
    ctxpriv = ndrx_ctx_priv_get();
    
    /* create xid first */
    
    if (NULL==xid)
    {
        NDRX_LOG(log_error, "Failed to convert C xid to Java");
        ret = XAER_RMERR;
        goto out;
    }
    
    /* Get the class for the context object */
    ctxClass = (*(JNIEnv *)ctxpriv->integptr1)->GetObjectClass((JNIEnv *)ctxpriv->integptr1, 
        (jobject)ctxpriv->integptr2);
    
    if (NULL==ctxClass)
    {
        NDRX_LOG(log_error, "Failed to get ctx object class");
        ret = XAER_RMERR;
        goto out;
    }
    
    /* Get the method id */
    mid = (*(JNIEnv *)ctxpriv->integptr1)->GetMethodID((JNIEnv *)ctxpriv->integptr1, 
        ctxClass, "xa_recover_entry", "(J)Lorg/endurox/XidList;");
    
    if (NULL==mid)
    {
        NDRX_LOG(log_error, "Failed to get xa_recover_entry() method!");
        ret = XAER_RMERR;
        goto out;
    }
    
    retObj = (*(JNIEnv *)ctxpriv->integptr1)->CallObjectMethod((JNIEnv *)ctxpriv->integptr1, 
        (jobject)ctxpriv->integptr2, mid, (jlong)flags);
    
    
    /* get xid class */
    xidClass = (*(JNIEnv *)ctxpriv->integptr1)->GetObjectClass((JNIEnv *)ctxpriv->integptr1, 
        retObj);
    
    if (NULL==xidClass)
    {
        NDRX_LOG(log_error, "Failed to get ctx object class");
        ret = XAER_RMERR;
        goto out;
    }
    
    /* get ret field */
    
    if (NULL==(fret = (*(JNIEnv *)ctxpriv->integptr1)->GetFieldID(
            (JNIEnv *)ctxpriv->integptr1, xidClass, "ret", "I")))
    {
        NDRX_LOG(log_error, "Failed to get ret field");
        ret = XAER_RMERR;
        goto out;
    }
    
    if (NULL==(fxidarr = (*(JNIEnv *)ctxpriv->integptr1)->GetFieldID(
            (JNIEnv *)ctxpriv->integptr1, xidClass, "list", "[Ljavax/transaction/xa/Xid;")))
    {
        NDRX_LOG(log_error, "Failed to get ret field");
        ret = XAER_RMERR;
        goto out;
    }
    
    /* get return code */
    ret = (*(JNIEnv *)ctxpriv->integptr1)->GetIntField((JNIEnv *)ctxpriv->integptr1, retObj, fret);
    
    if (XA_OK!=ret)
    {
        NDRX_LOG(log_error, "xa_recover returns %d error", ret);
        goto out;
    }
    
    xarr = (jarray)(*(JNIEnv *)ctxpriv->integptr1)->GetObjectField(
            (JNIEnv *)ctxpriv->integptr1, retObj, fxidarr);
    
    if (NULL!=xarr)
    {
        
        jcount = (*(JNIEnv *)ctxpriv->integptr1)->GetArrayLength((JNIEnv *)ctxpriv->integptr1, xarr);
        int minf = NDRX_MIN(count, jcount);
        int i;
        
        NDRX_LOG(log_debug, "Xids in returned from jdb: %d, but in c side: %d",
                jcount, count);
        
        for (i=0; i<minf; i++)
        {
            jobject jxid = (*(JNIEnv *)ctxpriv->integptr1)->GetObjectArrayElement
                    ((JNIEnv *)ctxpriv->integptr1, xarr, i);
            
            /* Convect java xid to C */
            if (EXSUCCEED!=ndrxj_cvt_xid_to_c((JNIEnv *)ctxpriv->integptr1, jxid, 
                    (xid+i)))
            {
                NDRX_LOG(log_error, "Failed to convert XID to C!");
                ret = XAER_RMERR;
                goto out;
            }
            ret++;
        }
    }
    
    NDRX_LOG(log_debug, "Recover OK");
    
out:
    NDRX_LOG(log_debug, "Java xa_recover_entry() returns %d", ret);
    
    if ((*(JNIEnv *)ctxpriv->integptr1)->ExceptionCheck((JNIEnv *)ctxpriv->integptr1))
    {
        NDRXJ_LOG_EXCEPTION(((JNIEnv *)ctxpriv->integptr1), log_error, NDRXJ_LOGEX_ULOG, 
                "xa_recover_entry() failed: %s");
        if (XA_OK==ret)
        {
            ret = XAER_RMERR;
        }
        (*(JNIEnv *)ctxpriv->integptr1)-> ExceptionClear((JNIEnv *)ctxpriv->integptr1);
    }

    /* clear up references... */
    if (NULL!=ctxClass)
    {
        (*(JNIEnv *)ctxpriv->integptr1)->DeleteLocalRef((JNIEnv *)ctxpriv->integptr1, 
                ctxClass);
    }

    if (NULL!=xidClass)
    {
        (*(JNIEnv *)ctxpriv->integptr1)->DeleteLocalRef((JNIEnv *)ctxpriv->integptr1, 
                xidClass);
    }

    if (NULL!=xarr)
    {
        (*(JNIEnv *)ctxpriv->integptr1)->DeleteLocalRef((JNIEnv *)ctxpriv->integptr1, 
               xarr);
    }

    return ret;
}

/**
 * Forget transaction
 * @param sw xa switch
 * @param xid XID
 * @param rmid RM ID
 * @param flags flags
 * @return XA_OK, XERR
 */
exprivate int xa_forget_entry(struct xa_switch_t *sw, XID *xid, int rmid, long flags)
{
    return xa_xid_entry("xa_forget_entry", sw, xid, rmid, flags);
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
    
    NDRX_LOG(log_error, "xa_complete_entry() - not supported!");
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
    return &ndrxjsw;
}


/* vim: set ts=4 sw=4 et smartindent: */
