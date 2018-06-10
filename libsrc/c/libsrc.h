/* 
** Commons for Enduro/X JAVA lib
**
** @file libsrc.h
** 
** -----------------------------------------------------------------------------
** Enduro/X Middleware Platform for Distributed Transaction Processing
** Copyright (C) 2015, Mavimax, Ltd. All Rights Reserved.
** This software is released under one of the following licenses:
** GPL or Mavimax's license for commercial use.
** -----------------------------------------------------------------------------
** GPL license:
** 
** This program is free software; you can redistribute it and/or modify it under
** the terms of the GNU General Public License as published by the Free Software
** Foundation; either version 2 of the License, or (at your option) any later
** version.
**
** This program is distributed in the hope that it will be useful, but WITHOUT ANY
** WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
** PARTICULAR PURPOSE. See the GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License along with
** this program; if not, write to the Free Software Foundation, Inc., 59 Temple
** Place, Suite 330, Boston, MA 02111-1307 USA
**
** -----------------------------------------------------------------------------
** A commercial use license is available from Mavimax, Ltd
** contact@mavimax.com
** -----------------------------------------------------------------------------
*/
/*---------------------------Includes-----------------------------------*/
#include <jni.h>
#include <atmi.h>
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/

/* Exception ops: */
extern void ndrxj_atmi_throw(JNIEnv *env, int err, char *msgfmt, ...);
extern void ndrxj_nstd_throw(JNIEnv *env, int err, char *msgfmt, ...);
extern void ndrxj_ubf_throw(JNIEnv *env, int err, char *msgfmt, ...);
extern TPCONTEXT_T ndrxj_get_ctx(JNIEnv *env, jobject atmiCtxObj, int do_set);
extern char *ndrxj_exception_backtrace(JNIEnv *env);

/* ClientId ops: */
extern jobject ndrxj_atmi_ClientId_translate(JNIEnv *env, 
            jobject ctx_obj, int is_ctxset, CLIENTID *cltid);

/* AtmiBuf ops: */
extern jobject ndrxj_atmi_AtmiBuf_translate(JNIEnv *env, 
            jobject ctx_obj, int is_ctxset, char *data, long len,
            char *type, char *subtype);

/* TpSvcInfo ops: */
extern jobject ndrxj_atmi_TpSvcInfo_translate(JNIEnv *env, 
            jobject ctx_obj, int is_ctxset, TPSVCINFO *svcinfo);




/* vim: set ts=4 sw=4 et cindent: */