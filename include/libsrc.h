/**
 * @brief Commons for Enduro/X JAVA lib
 *
 * @file libsrc.h
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

#ifndef NDRXJ_LIBSRC_H__
#define NDRXJ_LIBSRC_H__

#ifdef  __cplusplus
extern "C" {
#endif

/*---------------------------Includes-----------------------------------*/
#include <jni.h>
#include <atmi.h>
#include <ndrx_java_config.h>
#include <tmenv.h>
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/

/** NULL Logger -> no logging made with exception */
#define NDRXJ_LOGEX_NULL          0x0000
/** Log exception to ndrx logger */
#define NDRXJ_LOGEX_NDRX          0x0001
/** Log exception to ulog logger */
#define NDRXJ_LOGEX_ULOG          0x0002
/** Log exception to UBF logger */
#define NDRXJ_LOGEX_UBF           0x0004
/** Log exception to TP logger */
#define NDRXJ_LOGEX_TP            0x0008


#define NDRXJ_LOG_EXCEPTION_E(ENV__, EXC__, LEV__, FLAGS__, FMT__, ...) {\
\
    char *jerr__ = ndrxj_exception_backtrace(ENV__, EXC__);\
    char *jerr_null__ = "no JNI exception";\
    \
    if (NULL==jerr__) {jerr__= jerr_null__;}\
    if (FLAGS__ & NDRXJ_LOGEX_ULOG)\
        userlog(FMT__, ##__VA_ARGS__, jerr__);\
    if (FLAGS__ & NDRXJ_LOGEX_NDRX)\
        NDRX_LOG(LEV__, FMT__, ##__VA_ARGS__, jerr__);\
    if (FLAGS__ & NDRXJ_LOGEX_UBF)\
        UBF_LOG(LEV__, FMT__, ##__VA_ARGS__, jerr__);\
    if (FLAGS__ & NDRXJ_LOGEX_TP)\
        TP_LOG(LEV__, FMT__, ##__VA_ARGS__, jerr__);\
    NDRX_FREE(jerr__);\
}

/**
 * Log exception
 * @param ENV__ Java Env for which exception have occurred
 * @param LEV__ log level
 * @param ULOG__ use log EXTRUE/EXFALSE
 * @param FMT__ format string. Note first argument will be %s for stack trace
 * @param ... var arguments to format
 */
#define NDRXJ_LOG_EXCEPTION(ENV__, LEV__, FLAGS__, FMT__, ...) {\
\
    char *jerr__ = ndrxj_exception_backtrace(ENV__, NULL);\
    char *jerr_null__ = "no JNI exception";\
    \
    if (NULL==jerr__) {jerr__= jerr_null__;}\
    if (FLAGS__ & NDRXJ_LOGEX_ULOG)\
        userlog(FMT__, ##__VA_ARGS__, jerr__);\
    if (FLAGS__ & NDRXJ_LOGEX_NDRX)\
        NDRX_LOG(LEV__, FMT__, ##__VA_ARGS__, jerr__);\
    if (FLAGS__ & NDRXJ_LOGEX_UBF)\
        UBF_LOG(LEV__, FMT__, ##__VA_ARGS__, jerr__);\
    if (FLAGS__ & NDRXJ_LOGEX_TP)\
        TP_LOG(LEV__, FMT__, ##__VA_ARGS__, jerr__);\
    NDRX_FREE(jerr__);\
}

/**
 * Get java env from tls data
 */
#define NDRXJ_JENV(X)   ( ((JNIEnv *)(X->integptr1)) )
#define NDRXJ_JENV_LVAL(X)   X->integptr1
    
/**
 * Get ATMI Context from TLS data
 */
#define NDRXJ_JATMICTX(X)   ( ((jobject)(X->integptr2)) )
#define NDRXJ_JATMICTX_LVAL(X)   X->integptr2
    
/**
 * Get C Context from TLS data
 */
#define NDRXJ_CCTX(X)   ( ((TPCONTEXT_T)(X->integptr3)) )
#define NDRXJ_CCTX_LVAL(X)  X->integptr3
    
    
/**
 * The process is XATMI server including tmsrv
 */
#define NDRXJ_CTXFLAGS_SRV  0x00000001
/**
 * Context flags
 */
#define NDRXJ_CTXFLAGS(X)   X->integlng4

/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/

/**
 *  Java/C Object mapper for simple types 
 */
struct exjobjmap
{
    jfieldID *fid;  /**< Cached field id                */
    char *field;    /**< field name                     */
    char *ftype;    /**< field type in Java             */
    long coffset;   /**< c field offset                 */
    int csz;        /**< c field size                   */
    long flags;     /**< Special flags                  */
};

/**
 *  Java/C Object mapper for simple types 
 */
typedef struct exjobjmap exjobjmap_t;


/**
 * Access to TID byte array
 */
typedef struct
{
    TPTRANID tid;
} TPTRANID_conv;


/**
 * Cache of classes
 */
struct exj_class_cache
{
    char *classStr;     /**< String name of the class           */
    jclass *calzz;      /**< pointer to class obj               */
    int global;         /**< is global cached used?             */
};

/**
 *  Java/C Object mapper for simple types 
 */
typedef struct exj_class_cache exj_class_cache_t;

/**
 * Cache of method id
 */
struct exj_mid_cache
{
    jclass *calzz;      /**< pointer to class obj               */
    char *clz_ref;      /**< class debug reference              */
    jmethodID *mid;     /**< Method id                          */
    char *method;       /**< Method name                        */
    char *sign;         /**< Method signature                   */
};

/**
 *  Method ID cache
 */
typedef struct exj_mid_cache exj_mid_cache_t;

/**
 * Cache of field id
 */
struct exj_fid_cache
{
    jclass *calzz;      /**< pointer to class obj               */
    char *clz_ref;      /**< class debug reference              */
    jfieldID *fid;      /**< Method id                          */
    char *field;        /**< Method name                        */
    char *type;         /**< Method signature                   */
};

/**
 *  Method ID cache
 */
typedef struct exj_fid_cache exj_fid_cache_t;

/*---------------------------Globals------------------------------------*/

/*********************** AtmiCtx resources ************************************/
extern jmethodID ndrxj_clazz_AtmiCtx_mid_tpCallDispatch;
extern jmethodID ndrxj_clazz_AtmiCtx_mid_xa_open_entry;
extern jmethodID ndrxj_clazz_AtmiCtx_mid_xa_close_entry;
extern jmethodID ndrxj_clazz_AtmiCtx_mid_xa_end_entry;
extern jmethodID ndrxj_clazz_AtmiCtx_mid_xa_rollback_entry;
extern jmethodID ndrxj_clazz_AtmiCtx_mid_xa_prepare_entry;
extern jmethodID ndrxj_clazz_AtmiCtx_mid_xa_start_entry;
extern jmethodID ndrxj_clazz_AtmiCtx_mid_xa_commit_entry;
extern jmethodID ndrxj_clazz_AtmiCtx_mid_xa_recover_entry;
extern jmethodID ndrxj_clazz_AtmiCtx_mid_xa_forget_entry;

extern jfieldID ndrxj_clazz_AtmiCtx_fid_ctx;
extern jfieldID ndrxj_clazz_AtmiCtx_fid_svr;

/************************ Server resources ************************************/
extern jclass ndrxj_clazz_Server;
extern jmethodID ndrxj_clazz_Server_mid_tpSvrDone;
extern jmethodID ndrxj_clazz_Server_mid_tpSvrInit;

/************************ TpSvcInfo resources *********************************/
extern jclass ndrxj_clazz_TpSvcInfo;
extern jmethodID ndrxj_clazz_TpSvcInfo_mid_INIT;

/************************ TypedBuffer resources *******************************/
extern jclass ndrxj_clazz_TypedBuffer;
extern jmethodID ndrxj_clazz_TypedBuffer_mid_INIT;
extern jfieldID ndrxj_clazz_TypedBuffer_fid_doFinalize;
extern jfieldID ndrxj_clazz_TypedBuffer_fid_cPtr;
extern jfieldID ndrxj_clazz_TypedBuffer_fid_len;
extern jfieldID ndrxj_clazz_TypedBuffer_fid_ctx;

/************************ TypedUbf resources **********************************/
extern jclass ndrxj_clazz_TypedUbf;
extern jmethodID ndrxj_clazz_TypedUbf_mid_INIT;

/************************ TypedCarray resources *******************************/
extern jclass ndrxj_clazz_TypedCarray;
extern jmethodID ndrxj_clazz_TypedCarray_mid_INIT;

/************************ TypedString resources *******************************/
extern jclass ndrxj_clazz_TypedString;
extern jmethodID ndrxj_clazz_TypedString_mid_INIT;

/************************ TypedView resources *********************************/
extern jclass ndrxj_clazz_TypedView;
extern jmethodID ndrxj_clazz_TypedView_mid_INIT;

/************************ TypedJson resources *********************************/
extern jclass ndrxj_clazz_TypedJson;
extern jmethodID ndrxj_clazz_TypedJson_mid_INIT;

/************************ TypedTpInit resources *******************************/
extern jclass ndrxj_clazz_TypedTpInit;
extern jmethodID ndrxj_clazz_TypedTpInit_mid_INIT;

/************************ ClientId resources **********************************/
extern jclass ndrxj_clazz_ClientId;
extern jmethodID ndrxj_clazz_ClientId_mid_INIT;
extern jfieldID ndrxj_clazz_ClientId_fid_clientData;


/************************ ErrorTuple resources ********************************/
extern jclass ndrxj_clazz_ErrorTuple;
/* Methods: */
extern jmethodID ndrxj_clazz_ErrorTuple_mid_INIT;
/* Fields: */
extern jfieldID ndrxj_clazz_ErrorTuple_fid_err;
extern jfieldID ndrxj_clazz_ErrorTuple_fid_msg;

/************************ TPTRANID resources **********************************/
extern jclass ndrxj_clazz_TPTRANID;
extern jmethodID ndrxj_clazz_TPTRANID_mid_INIT;
extern jfieldID ndrxj_clazz_TPTRANID_fid_data;

/************************** TPQCTL resources **********************************/
extern jclass ndrxj_clazz_TPQCTL;
/* Methods: */
extern jmethodID ndrxj_clazz_TPQCTL_mid_INIT;
/* Fields: */
extern jfieldID ndrxj_clazz_TPQCTL_fid_flags;
extern jfieldID ndrxj_clazz_TPQCTL_fid_priority;
extern jfieldID ndrxj_clazz_TPQCTL_fid_diagnostic;
extern jfieldID ndrxj_clazz_TPQCTL_fid_diagmsg;
extern jfieldID ndrxj_clazz_TPQCTL_fid_msgid;
extern jfieldID ndrxj_clazz_TPQCTL_fid_corrid;
extern jfieldID ndrxj_clazz_TPQCTL_fid_replyqueue;
extern jfieldID ndrxj_clazz_TPQCTL_fid_failurequeue;
extern jfieldID ndrxj_clazz_TPQCTL_fid_urcode;
extern jfieldID ndrxj_clazz_TPQCTL_fid_appkey;
extern jfieldID ndrxj_clazz_TPQCTL_fid_delivery_qos;
extern jfieldID ndrxj_clazz_TPQCTL_fid_reply_qos;
extern jfieldID ndrxj_clazz_TPQCTL_fid_exp_time;
extern jfieldID ndrxj_clazz_TPQCTL_fid_cltid;

/************************** XidList resources *********************************/
extern jclass ndrxj_clazz_XidList;
extern jfieldID ndrxj_clazz_XidList_fid_ret;
extern jfieldID ndrxj_clazz_XidList_fid_list;

/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/

/* Exception ops: */
extern void ndrxj_atmi_throw(JNIEnv *env, jobject data, jobject addarg1, int err, char *msgfmt, ...);
extern void ndrxj_nstd_throw(JNIEnv *env, int err, char *msgfmt, ...);
extern void ndrxj_ubf_throw(JNIEnv *env, int err, char *msgfmt, ...);
extern TPCONTEXT_T ndrxj_get_ctx(JNIEnv *env, jobject atmiCtxObj, int do_set);
extern char *ndrxj_exception_backtrace(JNIEnv *env, jthrowable exc_in);

/* ClientId ops: */
extern jobject ndrxj_atmi_ClientId_translate(JNIEnv *env, 
            jobject ctx_obj, int is_ctxset, CLIENTID *cltid);

extern int ndrxj_atmi_ClientId_translate_toc(JNIEnv *env, 
        jobject in_jcltid, CLIENTID *out_cltid);

/* TypedBuffer ops: */

extern jobject ndrxj_atmi_TypedBuffer_translate(JNIEnv *env, 
            jobject ctx_obj, int is_ctxset, char *data, long len,
            char *type, char *subtype, int finalize);

extern TPCONTEXT_T ndrxj_TypedBuffer_get_ctx(JNIEnv *env, 
        jobject atmiBufObj, int do_set);

extern jobject ndrxj_atmi_TypedBuffer_result_prep
  (JNIEnv *env, jobject ctx_obj, jobject data, char *idata, 
        long ilen, char *odata, long olen, char *itype, char *isubtype);

extern int ndrxj_TypedBuffer_finalize_transfer(JNIEnv *env, 
        jobject to_data, jobject from_data, int from_invalidate);

extern int ndrxj_atmi_TypedBuffer_get_buffer(JNIEnv *env, 
            jobject data, char **buf, long *len, jboolean *doFinalize, 
            int unsetDoFinalize, int unsetPtr);

extern int ndrxj_atmi_TypedBuffer_set_buffer(JNIEnv *env, 
            jobject data, char *buf, long len);

/* TpCallResult ops: */

extern jobject ndrxj_atmi_TpCallResult_new(JNIEnv *env, 
        jobject ctx_obj, jobject dataObj, long tprucode);

/* TpSvcInfo ops: */
extern jobject ndrxj_atmi_TpSvcInfo_translate(JNIEnv *env, 
            jobject ctx_obj, int is_ctxset, TPSVCINFO *svcinfo,
            jobject *p_jdata, jobject *p_jcltid, jstring *p_jname, 
            jstring *p_jfname);


/* BFldLocInfo ops */
extern BFLDID* ndrxj_BFldLocInfo_ptr_get(JNIEnv *env, jobject loc);
extern void ndrxj_BFldLocInfo_ptr_set(JNIEnv *env, jobject loc, BFLDID *new_ptr);

/* BExprTree ops */

extern char* ndrxj_BExprTree_ptr_get(JNIEnv *env, jobject ptrO);
extern void ndrxj_BExprTree_ptr_set(JNIEnv *env, jobject ptrO, char *new_ptr);
extern jobject ndrxj_BExprTree_new(JNIEnv *env, char *ptr);

/* Bbool ops */
extern long Bbool_callback_function(UBFH *p_ub, char *funcname);

/* Bnext ops */
extern jobject ndrxj_BNextResult_new(JNIEnv *env, 
        BFLDID bfldid, BFLDOCC occ, BFLDLEN len);

/* TpgetrplyResul ops */
extern jobject ndrxj_TpgetrplyResult_new(JNIEnv *env, jobject ctx_obj,
        int cd, jobject odata);

/* TpTypesResult ops */
extern jobject ndrxj_TpTypesResult_new(JNIEnv *env, 
        char *type, char *sub_type, long size);

/* TprecvResult ops */
extern jobject ndrxj_TprecvResult_new(JNIEnv *env, jobject ctx_obj,
        int cd, jobject odata, long revent);

/* Unsol handling */

extern void ndrxj_atmictx_unsol_globals_set(JNIEnv* env, jobject atmiCtxObj);

/* Utils */
extern int ndrxj_cvt_to_c(JNIEnv *env, 
            jobject ctx_obj, exjobjmap_t *tab, char *clzstr,
            jobject jobj, void *cobj);


extern int ndrxj_cvt_to_java(JNIEnv *env, 
            jobject ctx_obj, exjobjmap_t *tab, char *clzstr,
            void *cobj, jobject jobj);

extern int ndrxj_cvt_jstr_to_c(JNIEnv *env, 
            jobject ctx_obj, jstring jstr, char *outstr, int outstrsz);

/* TPQCTL */

extern int ndrxj_atmi_TPQCTL_translate2c(JNIEnv *env, 
            jobject ctx_obj, jobject ctl_Java, TPQCTL *ctl_c);


extern jobject ndrxj_atmi_TPQCTL_translate2java(JNIEnv *env, 
            jobject ctx_obj, jobject ctl_Java, TPQCTL *ctl_c);

/* TPTRANID */

extern int ndrxj_atmi_TPTRANID_translate2c(JNIEnv *env, 
            jobject ctx_obj, jobject tid_Java, TPTRANID_conv *tid_c);
extern jobject ndrxj_atmi_TPTRANID_translate2java(JNIEnv *env, 
            jobject ctx_obj, jobject tid_Java, TPTRANID_conv *tid_c);

/* XA Driver */

extern int ndrxj_xa_cfgparse(char *buffer, string_list_t **sets, int *nrsets, 
        char *clazz, int clazz_bufsz);

extern jobjectArray ndrxj_cvt_arr_c_to_java(JNIEnv *env, string_list_t *list, int nrel);
extern jobject ndrxj_cvt_xid_to_java(JNIEnv *env, XID *xid);
extern int ndrxj_cvt_xid_to_c(JNIEnv *env, jobject j_xid, XID *c_xid);

/* TODO: How about dllimport/export? */
extern NDRX_JAVA_API int ndrxj_xa_init(void);
extern NDRX_JAVA_API int ndrxj_jvm_create(ndrx_ctx_priv_t *ctxpriv);
extern NDRX_JAVA_API int ndrxj_alloc_context(ndrx_ctx_priv_t *ctxpriv);

extern NDRX_JAVA_API int ndrxj_caches_load(JNIEnv *env);
extern NDRX_JAVA_API void ndrxj_caches_unload(JNIEnv *env);
#ifdef  __cplusplus
}
#endif

#endif  /* NDRXJ_LIBSRC_H__ */


/* vim: set ts=4 sw=4 et smartindent: */
