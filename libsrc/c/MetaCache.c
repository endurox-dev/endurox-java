/**
 * @brief Meta data cache registry
 *
 * @file MetaCache.c
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
#include "exjglue.h"
#include <atmi.h>
#include <oatmi.h>
#include <ndebug.h>
#include <ondebug.h>
#include <oatmisrv_integra.h>
#include <libsrc.h>
#include <sys_unix.h>
#include <tmenv.h>
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
/* Class reference: */
#define CRF(X) &X, #X
#define GLOB    EXTRUE
#define LOCL    EXFALSE
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/

/*********************** AtmiCtx resources ************************************/
expublic jclass ndrxj_clazz_AtmiCtx;

/* Methods: */
expublic jmethodID ndrxj_clazz_AtmiCtx_mid_tpCallDispatch;
expublic jmethodID ndrxj_clazz_AtmiCtx_mid_xa_open_entry;
expublic jmethodID ndrxj_clazz_AtmiCtx_mid_xa_close_entry;
expublic jmethodID ndrxj_clazz_AtmiCtx_mid_xa_end_entry;
expublic jmethodID ndrxj_clazz_AtmiCtx_mid_xa_rollback_entry;
expublic jmethodID ndrxj_clazz_AtmiCtx_mid_xa_prepare_entry;
expublic jmethodID ndrxj_clazz_AtmiCtx_mid_xa_start_entry;
expublic jmethodID ndrxj_clazz_AtmiCtx_mid_xa_commit_entry;
expublic jmethodID ndrxj_clazz_AtmiCtx_mid_xa_recover_entry;
expublic jmethodID ndrxj_clazz_AtmiCtx_mid_xa_forget_entry;
expublic jmethodID ndrxj_clazz_AtmiCtx_mid_unsolDispatch;

        
/* Fields: */
expublic jfieldID ndrxj_clazz_AtmiCtx_fid_ctx;
expublic jfieldID ndrxj_clazz_AtmiCtx_fid_svr;
        
/************************ Server resources ************************************/
expublic jclass ndrxj_clazz_Server;
/* Methods: */
expublic jmethodID ndrxj_clazz_Server_mid_tpSvrDone;
expublic jmethodID ndrxj_clazz_Server_mid_tpSvrInit;

/* Fields: */


/************************ TpSvcInfo resources *********************************/
expublic jclass ndrxj_clazz_TpSvcInfo;
/* Methods: */
expublic jmethodID ndrxj_clazz_TpSvcInfo_mid_INIT;

/* Fields: */
        
/************************ TypedBuffer resources *******************************/
expublic jclass ndrxj_clazz_TypedBuffer;
/* Methods: */
expublic jmethodID ndrxj_clazz_TypedBuffer_mid_INIT;
/* Fields: */
expublic jfieldID ndrxj_clazz_TypedBuffer_fid_doFinalize;
expublic jfieldID ndrxj_clazz_TypedBuffer_fid_cPtr;
expublic jfieldID ndrxj_clazz_TypedBuffer_fid_len;
expublic jfieldID ndrxj_clazz_TypedBuffer_fid_ctx;


/************************ TypedUbf resources **********************************/
expublic jclass ndrxj_clazz_TypedUbf;
/* Methods: */
expublic jmethodID ndrxj_clazz_TypedUbf_mid_INIT;
/* Fields: */

/************************ TypedCarray resources *******************************/
expublic jclass ndrxj_clazz_TypedCarray;
/* Methods: */
expublic jmethodID ndrxj_clazz_TypedCarray_mid_INIT;
/* Fields: */

/************************ TypedString resources *******************************/
expublic jclass ndrxj_clazz_TypedString;
/* Methods: */
expublic jmethodID ndrxj_clazz_TypedString_mid_INIT;
/* Fields: */

/************************ TypedView resources *********************************/
expublic jclass ndrxj_clazz_TypedView;
/* Methods: */
expublic jmethodID ndrxj_clazz_TypedView_mid_INIT;
/* Fields: */

/************************ TypedJson resources *********************************/
expublic jclass ndrxj_clazz_TypedJson;
/* Methods: */
expublic jmethodID ndrxj_clazz_TypedJson_mid_INIT;
/* Fields: */

/************************ TypedTpInit resources *******************************/
expublic jclass ndrxj_clazz_TypedTpInit;
/* Methods: */
expublic jmethodID ndrxj_clazz_TypedTpInit_mid_INIT;
/* Fields: */

/************************ ClientId resources **********************************/
expublic jclass ndrxj_clazz_ClientId;
/* Methods: */
expublic jmethodID ndrxj_clazz_ClientId_mid_INIT;
/* Fields: */
expublic jfieldID ndrxj_clazz_ClientId_fid_clientData;

/************************ ErrorTuple resources ********************************/
expublic jclass ndrxj_clazz_ErrorTuple;
/* Methods: */
expublic jmethodID ndrxj_clazz_ErrorTuple_mid_INIT;
/* Fields: */
expublic jfieldID ndrxj_clazz_ErrorTuple_fid_err;
expublic jfieldID ndrxj_clazz_ErrorTuple_fid_msg;

/************************ TPTRANID resources **********************************/
expublic jclass ndrxj_clazz_TPTRANID;
/* Methods: */
expublic jmethodID ndrxj_clazz_TPTRANID_mid_INIT;
/* Fields: */
expublic jfieldID ndrxj_clazz_TPTRANID_fid_data;

/************************** TPQCTL resources **********************************/
expublic jclass ndrxj_clazz_TPQCTL;
/* Methods: */
expublic jmethodID ndrxj_clazz_TPQCTL_mid_INIT;
/* Fields: */
expublic jfieldID ndrxj_clazz_TPQCTL_fid_flags;
expublic jfieldID ndrxj_clazz_TPQCTL_fid_priority;
expublic jfieldID ndrxj_clazz_TPQCTL_fid_diagnostic;
expublic jfieldID ndrxj_clazz_TPQCTL_fid_diagmsg;
expublic jfieldID ndrxj_clazz_TPQCTL_fid_msgid;
expublic jfieldID ndrxj_clazz_TPQCTL_fid_corrid;
expublic jfieldID ndrxj_clazz_TPQCTL_fid_replyqueue;
expublic jfieldID ndrxj_clazz_TPQCTL_fid_failurequeue;
expublic jfieldID ndrxj_clazz_TPQCTL_fid_urcode;
expublic jfieldID ndrxj_clazz_TPQCTL_fid_appkey;
expublic jfieldID ndrxj_clazz_TPQCTL_fid_delivery_qos;
expublic jfieldID ndrxj_clazz_TPQCTL_fid_reply_qos;
expublic jfieldID ndrxj_clazz_TPQCTL_fid_exp_time;
expublic jfieldID ndrxj_clazz_TPQCTL_fid_cltid;

/************************** XidList resources *********************************/
expublic jclass ndrxj_clazz_XidList;
/* Methods: */
/* Fields: */
expublic jfieldID ndrxj_clazz_XidList_fid_ret;
expublic jfieldID ndrxj_clazz_XidList_fid_list;

/************************** Xid resources *************************************/
expublic jclass ndrxj_clazz_Xid;
/* Methods: */

expublic jmethodID ndrxj_clazz_Xid_mid_getFormatId;
expublic jmethodID ndrxj_clazz_Xid_mid_getBranchQualifier;
expublic jmethodID ndrxj_clazz_Xid_mid_getGlobalTransactionId;

/* Fields: */

/************************** XidList resources *********************************/

expublic jclass ndrxj_clazz_ExXid;
/* Methods: */
expublic jmethodID ndrxj_clazz_ExXid_mid_INIT;
/* Fields: */

/************************** String resources **********************************/
expublic jclass ndrxj_clazz_String;
/* Methods: */
/* Fields: */

/************************** OutputStream resources ****************************/
expublic jclass ndrxj_clazz_OutputStream;
/* Methods: */
expublic jmethodID ndrxj_clazz_OutputStream_mid_write;
/* Fields: */

/************************** BExprTree resources *******************************/
expublic jclass ndrxj_clazz_BExprTree;
/* Methods: */
expublic jmethodID ndrxj_clazz_BExprTree_mid_INIT;
/* Fields: */
expublic jfieldID ndrxj_clazz_BExprTree_fid_cPtr;

/************************** BNextResult resources *******************************/
expublic jclass ndrxj_clazz_BNextResult;
/* Methods: */
expublic jmethodID ndrxj_clazz_BNextResult_mid_INIT;
/* Fields: */

/**
 * Mapping of the classes object
 */
exprivate exj_class_cache_t M_classes[] =
{ 
    {"org/endurox/AtmiCtx", &ndrxj_clazz_AtmiCtx,                   LOCL}
    ,{"org/endurox/Server", &ndrxj_clazz_Server,                    LOCL}
    ,{"org/endurox/TpSvcInfo", &ndrxj_clazz_TpSvcInfo,              GLOB}
    ,{"org/endurox/TypedBuffer", &ndrxj_clazz_TypedBuffer,          GLOB}
    ,{"org/endurox/TypedUbf", &ndrxj_clazz_TypedUbf,                GLOB}
    ,{"org/endurox/TypedCarray", &ndrxj_clazz_TypedCarray,          GLOB}
    ,{"org/endurox/TypedString", &ndrxj_clazz_TypedString,          GLOB}
    ,{"org/endurox/TypedView", &ndrxj_clazz_TypedView,              GLOB}
    ,{"org/endurox/TypedJson", &ndrxj_clazz_TypedJson,              GLOB}
    ,{"org/endurox/TypedTpInit", &ndrxj_clazz_TypedTpInit,          GLOB}
    ,{"org/endurox/ClientId", &ndrxj_clazz_ClientId,                GLOB}
    ,{"org/endurox/ErrorTuple", &ndrxj_clazz_ErrorTuple,            GLOB}
    ,{"org/endurox/TPTRANID", &ndrxj_clazz_TPTRANID,                GLOB}
    ,{"org/endurox/TPQCTL", &ndrxj_clazz_TPQCTL,                    GLOB}
    ,{"org/endurox/XidList", &ndrxj_clazz_XidList,                  LOCL}
    ,{"javax/transaction/xa/Xid", &ndrxj_clazz_Xid,                 LOCL}
    ,{"java/lang/String", &ndrxj_clazz_String,                      GLOB}
    ,{"org/endurox/ExXid", &ndrxj_clazz_ExXid,                      GLOB}
    ,{"java/io/OutputStream", &ndrxj_clazz_OutputStream,            LOCL}
    ,{"org/endurox/BExprTree", &ndrxj_clazz_BExprTree,              GLOB}
    ,{"org/endurox/BNextResult", &ndrxj_clazz_BNextResult,          GLOB}
    
    
};

/**
 * Mappings of methods cached
 */
exprivate exj_mid_cache_t M_methods[] =
{ 
    {CRF(ndrxj_clazz_AtmiCtx), &ndrxj_clazz_AtmiCtx_mid_tpCallDispatch, 
            "tpCallDispatch", "(Lorg/endurox/TpSvcInfo;)V"}
    ,{CRF(ndrxj_clazz_AtmiCtx), &ndrxj_clazz_AtmiCtx_mid_xa_open_entry, 
            "xa_open_entry", "(J)I"}
    ,{CRF(ndrxj_clazz_AtmiCtx), &ndrxj_clazz_AtmiCtx_mid_xa_close_entry, 
            "xa_close_entry", "(J)I"}
    
    ,{CRF(ndrxj_clazz_AtmiCtx), &ndrxj_clazz_AtmiCtx_mid_xa_end_entry, 
            "xa_end_entry", "(Ljavax/transaction/xa/Xid;J)I"}
    ,{CRF(ndrxj_clazz_AtmiCtx), &ndrxj_clazz_AtmiCtx_mid_xa_rollback_entry, 
            "xa_rollback_entry", "(Ljavax/transaction/xa/Xid;J)I"}
    ,{CRF(ndrxj_clazz_AtmiCtx), &ndrxj_clazz_AtmiCtx_mid_xa_prepare_entry, 
            "xa_prepare_entry", "(Ljavax/transaction/xa/Xid;J)I"}
    ,{CRF(ndrxj_clazz_AtmiCtx), &ndrxj_clazz_AtmiCtx_mid_xa_start_entry, 
            "xa_start_entry", "(Ljavax/transaction/xa/Xid;J)I"}
    ,{CRF(ndrxj_clazz_AtmiCtx), &ndrxj_clazz_AtmiCtx_mid_xa_commit_entry, 
            "xa_commit_entry", "(Ljavax/transaction/xa/Xid;J)I"}
    ,{CRF(ndrxj_clazz_AtmiCtx), &ndrxj_clazz_AtmiCtx_mid_xa_recover_entry, 
            "xa_recover_entry", "(J)Lorg/endurox/XidList;"}
    
    ,{CRF(ndrxj_clazz_AtmiCtx), &ndrxj_clazz_AtmiCtx_mid_xa_forget_entry, 
            "xa_forget_entry", "(Ljavax/transaction/xa/Xid;J)I"}
    
    ,{CRF(ndrxj_clazz_AtmiCtx), &ndrxj_clazz_AtmiCtx_mid_xa_forget_entry, 
            "unsolDispatch", "(Lorg/endurox/TypedBuffer;J)V"}
    
    ,{CRF(ndrxj_clazz_Server), &ndrxj_clazz_Server_mid_tpSvrDone, 
            "tpSvrDone", "(Lorg/endurox/AtmiCtx;)V"}
    ,{CRF(ndrxj_clazz_Server), &ndrxj_clazz_Server_mid_tpSvrInit, 
            "tpSvrInit", "(Lorg/endurox/AtmiCtx;[Ljava/lang/String;)I"}
    ,{CRF(ndrxj_clazz_TpSvcInfo), &ndrxj_clazz_TpSvcInfo_mid_INIT, "<init>", 
            "(Ljava/lang/String;Lorg/endurox/TypedBuffer;JIJLorg/endurox/ClientId;Ljava/lang/String;)V"}
    ,{CRF(ndrxj_clazz_TypedBuffer), &ndrxj_clazz_TypedBuffer_mid_INIT, "<init>", 
            "(Lorg/endurox/AtmiCtx;ZJJ)V"}
    ,{CRF(ndrxj_clazz_TypedUbf), &ndrxj_clazz_TypedUbf_mid_INIT, "<init>", "(Lorg/endurox/AtmiCtx;ZJJ)V"}
    ,{CRF(ndrxj_clazz_TypedCarray), &ndrxj_clazz_TypedCarray_mid_INIT, "<init>", "(Lorg/endurox/AtmiCtx;ZJJ)V"}
    ,{CRF(ndrxj_clazz_TypedString), &ndrxj_clazz_TypedString_mid_INIT, "<init>", "(Lorg/endurox/AtmiCtx;ZJJ)V"}
    ,{CRF(ndrxj_clazz_TypedView), &ndrxj_clazz_TypedView_mid_INIT, "<init>", "(Lorg/endurox/AtmiCtx;ZJJ)V"}
    ,{CRF(ndrxj_clazz_TypedJson), &ndrxj_clazz_TypedJson_mid_INIT, "<init>", "(Lorg/endurox/AtmiCtx;ZJJ)V"}
    ,{CRF(ndrxj_clazz_TypedTpInit), &ndrxj_clazz_TypedTpInit_mid_INIT, "<init>", "(Lorg/endurox/AtmiCtx;ZJJ)V"}
    ,{CRF(ndrxj_clazz_ClientId), &ndrxj_clazz_ClientId_mid_INIT, "<init>", "(Ljava/lang/String;)V"}
    ,{CRF(ndrxj_clazz_ErrorTuple), &ndrxj_clazz_ErrorTuple_mid_INIT, "<init>", "()V"}
    ,{CRF(ndrxj_clazz_TPTRANID), &ndrxj_clazz_TPTRANID_mid_INIT, "<init>", "()V"}
    ,{CRF(ndrxj_clazz_TPQCTL), &ndrxj_clazz_TPQCTL_mid_INIT, "<init>", "()V"}
    ,{CRF(ndrxj_clazz_ExXid), &ndrxj_clazz_ExXid_mid_INIT, "<init>", "(JJJ[B)V"}
    ,{CRF(ndrxj_clazz_Xid), &ndrxj_clazz_Xid_mid_getFormatId, "getFormatId", "()I"}
    ,{CRF(ndrxj_clazz_Xid), &ndrxj_clazz_Xid_mid_getBranchQualifier, "getBranchQualifier", "()[B"}
    ,{CRF(ndrxj_clazz_Xid), &ndrxj_clazz_Xid_mid_getGlobalTransactionId, "getGlobalTransactionId", "()[B"}
    ,{CRF(ndrxj_clazz_OutputStream), &ndrxj_clazz_OutputStream_mid_write, "write", "([B)V"}
    ,{CRF(ndrxj_clazz_BExprTree), &ndrxj_clazz_BExprTree_mid_INIT, "<init>", "(J)V"}
    ,{CRF(ndrxj_clazz_BNextResult), &ndrxj_clazz_BNextResult_mid_INIT, "<init>", "(III)V"}
    
};

/**
 * Mappings of fields cached
 */
exprivate exj_fid_cache_t M_fields[] =
{ 
    {CRF(ndrxj_clazz_AtmiCtx), &ndrxj_clazz_AtmiCtx_fid_ctx, "ctx", "J"}
    ,{CRF(ndrxj_clazz_AtmiCtx), &ndrxj_clazz_AtmiCtx_fid_svr, "svr", "Lorg/endurox/Server;"}
    ,{CRF(ndrxj_clazz_TypedBuffer), &ndrxj_clazz_TypedBuffer_fid_doFinalize, "doFinalize", "Z"}
    ,{CRF(ndrxj_clazz_TypedBuffer), &ndrxj_clazz_TypedBuffer_fid_cPtr, "cPtr", "J"}
    ,{CRF(ndrxj_clazz_TypedBuffer), &ndrxj_clazz_TypedBuffer_fid_len, "len", "J"}
    ,{CRF(ndrxj_clazz_TypedBuffer), &ndrxj_clazz_TypedBuffer_fid_ctx, "ctx", 
            "Lorg/endurox/AtmiCtx;"}
    ,{CRF(ndrxj_clazz_ClientId), &ndrxj_clazz_ClientId_fid_clientData, "clientData", 
            "Ljava/lang/String;"}
    ,{CRF(ndrxj_clazz_ErrorTuple), &ndrxj_clazz_ErrorTuple_fid_err, "err", 
            "I"}
    ,{CRF(ndrxj_clazz_ErrorTuple), &ndrxj_clazz_ErrorTuple_fid_msg, "msg", 
            "Ljava/lang/String;"}
   ,{CRF(ndrxj_clazz_TPTRANID), &ndrxj_clazz_TPTRANID_fid_data, "data", "[B"}
   ,{CRF(ndrxj_clazz_TPQCTL), &ndrxj_clazz_TPQCTL_fid_flags, "flags", "J"}
   ,{CRF(ndrxj_clazz_TPQCTL), &ndrxj_clazz_TPQCTL_fid_priority, "priority", "J"}
   ,{CRF(ndrxj_clazz_TPQCTL), &ndrxj_clazz_TPQCTL_fid_diagnostic, "diagnostic", "J"}
   ,{CRF(ndrxj_clazz_TPQCTL), &ndrxj_clazz_TPQCTL_fid_diagmsg, "diagmsg", "Ljava/lang/String;"}
   ,{CRF(ndrxj_clazz_TPQCTL), &ndrxj_clazz_TPQCTL_fid_msgid, "msgid", "[B"}
   ,{CRF(ndrxj_clazz_TPQCTL), &ndrxj_clazz_TPQCTL_fid_corrid, "corrid", "[B"}
   ,{CRF(ndrxj_clazz_TPQCTL), &ndrxj_clazz_TPQCTL_fid_replyqueue, "replyqueue", "Ljava/lang/String;"}
   ,{CRF(ndrxj_clazz_TPQCTL), &ndrxj_clazz_TPQCTL_fid_failurequeue, "failurequeue", "Ljava/lang/String;"}
   ,{CRF(ndrxj_clazz_TPQCTL), &ndrxj_clazz_TPQCTL_fid_urcode, "urcode", "J"}
   ,{CRF(ndrxj_clazz_TPQCTL), &ndrxj_clazz_TPQCTL_fid_appkey, "appkey", "J"}
   ,{CRF(ndrxj_clazz_TPQCTL), &ndrxj_clazz_TPQCTL_fid_delivery_qos, "delivery_qos", "J"}
   ,{CRF(ndrxj_clazz_TPQCTL), &ndrxj_clazz_TPQCTL_fid_reply_qos, "reply_qos", "J"}
   ,{CRF(ndrxj_clazz_TPQCTL), &ndrxj_clazz_TPQCTL_fid_exp_time, "exp_time", "J"}
   ,{CRF(ndrxj_clazz_TPQCTL), &ndrxj_clazz_TPQCTL_fid_cltid, "cltid", "Lorg/endurox/ClientId;"}
   ,{CRF(ndrxj_clazz_XidList), &ndrxj_clazz_XidList_fid_ret, "ret", "I"}
   ,{CRF(ndrxj_clazz_XidList), &ndrxj_clazz_XidList_fid_list, "list", "[Ljavax/transaction/xa/Xid;"}
   ,{CRF(ndrxj_clazz_XidList), &ndrxj_clazz_XidList_fid_list, "list", "[Ljavax/transaction/xa/Xid;"}
   ,{CRF(ndrxj_clazz_BExprTree), &ndrxj_clazz_BExprTree_fid_cPtr, "cPtr", "J"}
};

/*---------------------------Statics------------------------------------*/

/**
 * Load the caches
 * @return EXSUCCEED/EXFAIL
 */
expublic int ndrxj_caches_load(JNIEnv *env)
{
    int i;
    int ret = EXSUCCEED;
    
    NDRX_LOG(log_debug, "Loading class caches...");
    
    for (i=0; i<N_DIM(M_classes); i++)
    {
        *M_classes[i].calzz = (*env)->FindClass(env, M_classes[i].classStr);
        
        if (NULL==*M_classes[i].calzz)
        {
            NDRX_LOG(log_error, "Failed to find class [%s]", M_classes[i].classStr);
            userlog("Failed to find class [%s]", M_classes[i].classStr);
            EXFAIL_OUT(ret);
        }
        
        /* mk weak ref so that class can unload */
        
        if (M_classes[i].global)
        {
            *M_classes[i].calzz = (*env)->NewWeakGlobalRef(env, *M_classes[i].calzz);
        }
        
        if (NULL==*M_classes[i].calzz)
        {
            NDRX_LOG(log_error, "Failed to make globl ref [%s]", M_classes[i].classStr);
            userlog("Failed to make globl ref [%s]", M_classes[i].classStr);
            EXFAIL_OUT(ret);
        }
        
    }
    
    NDRX_LOG(log_debug, "Loading method caches...");
    
    for (i=0; i<N_DIM(M_methods); i++)
    {
        *M_methods[i].mid = (*env)->GetMethodID(env, *M_methods[i].calzz, 
                M_methods[i].method, M_methods[i].sign);
        
        if (NULL==*M_methods[i].mid)
        {
            NDRX_LOG(log_error, "Failed to find Method ID: %s:%s:%s", 
                    M_methods[i].clz_ref, M_methods[i].method, M_methods[i].sign);
            userlog("Failed to find Method ID: %s:%s:%s", 
                    M_methods[i].clz_ref, M_methods[i].method, M_methods[i].sign);
            EXFAIL_OUT(ret);
        }
    }

    NDRX_LOG(log_debug, "Loading field caches...");
    
    for (i=0; i<N_DIM(M_fields); i++)
    {
        *M_fields[i].fid = (*env)->GetFieldID(env, *M_fields[i].calzz, 
                M_fields[i].field, M_fields[i].type);
        
        if (NULL==*M_fields[i].fid)
        {
            NDRX_LOG(log_error, "Failed to find Field ID: %s:%s:%s", 
                    M_fields[i].clz_ref, M_fields[i].field, M_fields[i].type);
            userlog("Failed to find Field ID: %s:%s:%s", 
                    M_fields[i].clz_ref, M_fields[i].field, M_fields[i].type);
            EXFAIL_OUT(ret);
        }
    }
    
    NDRX_LOG(log_debug, "Removing class references");
    for (i=0; i<N_DIM(M_classes); i++)
    {
        if (!M_classes[i].global)
        {
            (*env)->DeleteLocalRef( env, *M_classes[i].calzz);
        }
    }
    
out:
    return ret;
}

/**
 * Remove any global class references if have
 * @param env java env
 */
expublic void ndrxj_caches_unload(JNIEnv *env)
{
    int i;
    
    NDRX_LOG(log_debug, "Removing global class references");
    
    for (i=0; i<N_DIM(M_classes); i++)
    {
        if (M_classes[i].global)
        {
            (*env)->DeleteLocalRef( env, *M_classes[i].calzz);
        }
    }
    
}

/* vim: set ts=4 sw=4 et smartindent: */
