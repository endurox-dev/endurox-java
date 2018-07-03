/**
 * @brief Commons for Enduro/X JAVA lib
 *
 * @file libsrc.c
 */ 
/*
 * -----------------------------------------------------------------------------
 * Enduro/X Middleware Platform for Distributed Transaction Processing
 * Copyright (C) 2015-2018 Mavimax, Ltd. All Rights Reserved.
 * This software is released under one of the following licenses:
 * GPL or Mavimax's license for commercial use.
 * -----------------------------------------------------------------------------
 * GPL license:
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
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
#include <atmi.h>
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/

/** Log exception to ndrx logger */
#define NDRXJ_LOGEX_NDRX          0x0001
/** Log exception to ulog logger */
#define NDRXJ_LOGEX_ULOG          0x0002


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
    char *jerr__ = ndrxj_exception_backtrace(ENV__);\
    \
    if (FLAGS__ & NDRXJ_LOGEX_NDRX)\
        userlog(FMT__, jerr__, ##__VA_ARGS__);\
    if (FLAGS__ & NDRXJ_LOGEX_ULOG)\
        NDRX_LOG(LEV__, FMT__, jerr__, ##__VA_ARGS__);\
    NDRX_FREE(jerr__);\
}
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/

/* Exception ops: */
extern void ndrxj_atmi_throw(JNIEnv *env, jobject data, int err, char *msgfmt, ...);
extern void ndrxj_nstd_throw(JNIEnv *env, int err, char *msgfmt, ...);
extern void ndrxj_ubf_throw(JNIEnv *env, int err, char *msgfmt, ...);
extern TPCONTEXT_T ndrxj_get_ctx(JNIEnv *env, jobject atmiCtxObj, int do_set);
extern char *ndrxj_exception_backtrace(JNIEnv *env);

/* ClientId ops: */
extern jobject ndrxj_atmi_ClientId_translate(JNIEnv *env, 
            jobject ctx_obj, int is_ctxset, CLIENTID *cltid);

/* TypedBuffer ops: */

extern jobject ndrxj_atmi_TypedBuffer_translate(JNIEnv *env, 
            jobject ctx_obj, int is_ctxset, char *data, long len,
            char *type, char *subtype);

extern TPCONTEXT_T ndrxj_TypedBuffer_get_ctx(JNIEnv *env, 
        jobject atmiBufObj, int do_set);

extern int ndrxj_atmi_TypedBuffer_get_buffer(JNIEnv *env, 
            jobject data, char **buf, long *len);

extern jobject ndrxj_atmi_TypedBuffer_result_prep
  (JNIEnv *env, jobject ctx_obj, jobject data, char *idata, 
        long ilen, char *odata, long olen);


/* TpCallResult ops: */

extern jobject ndrxj_atmi_TpCallResult_new(JNIEnv *env, 
        jobject ctx_obj, jobject dataObj, long tprucode);

/* TpSvcInfo ops: */
extern jobject ndrxj_atmi_TpSvcInfo_translate(JNIEnv *env, 
            jobject ctx_obj, int is_ctxset, TPSVCINFO *svcinfo);

/* vim: set ts=4 sw=4 et cindent: */