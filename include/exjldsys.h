/**
 * @brief Endruo/X Java Linker, static API, System
 *
 * @file exjldsys.h
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

#ifndef EXJLDSYS_H__
#define EXJLDSYS_H__

#ifdef  __cplusplus
extern "C" {
#endif


/*---------------------------Includes-----------------------------------*/
#include <stdlib.h>
#include <jni.h>
#include <exjlds.h>
#include <ndrx_java_config.h>
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/

extern NDRX_JAVA_API int ndrxj_ldr_get_static_handler(JNIEnv *env, 
			char *run_class_str,
                        char *static_method,
                        char *static_method_sign,
                        jclass *run_class,
                        jmethodID *run_mid
  			);

extern NDRX_JAVA_API JavaVM * ndrxj_ldr_getvm(ndrxj_class_index_t *class_index, 
	        int class_index_len, ndrxj_class_index_t *res_index, int res_index_len,
                JNIEnv **env);

#ifdef  __cplusplus
}
#endif

#endif  /* EXJLDSYS_H__ */


/* vim: set ts=4 sw=4 et smartindent: */
