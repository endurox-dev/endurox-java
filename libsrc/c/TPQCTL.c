/**
 * @brief TPQCTL structure translate from/to Java/C
 *
 * @file TPQCTL.c
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
#include "org_endurox_TPQCTL.h"
#include <atmi.h>
#include <oatmi.h>
#include <ndebug.h>
#include <ondebug.h>
#include <oatmisrv_integra.h>
#include "libsrc.h"
#include <sys_unix.h>
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
#define TPQCTL_CLASS     "org/endurox/TPQCTL"
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/

/**
 * Translate TPQCTL to C from java
 * @param env java env
 * @param ctx_obj ATMI Context
 * @param ctl_Java Java object
 * @param ctl_c C object
 * @return EXSUCCEED/EXFAIL
 */
expublic int ndrxj_atmi_TPQCTL_translate2c(JNIEnv *env, 
            jobject ctx_obj, jobject ctl_Java, TPQCTL *ctl_c)
{
    int ret = EXSUCCEED;
    
out:

    return ret;
}

/**
 * Translate C struct to Java
 * @param env java env
 * @param ctx_obj ATMI Ctx obj
 * @param ctl_c C Control struct
 * @return EXSUCCEED/EXFAIL
 */
expublic jobject ndrxj_atmi_TPQCTL_translate2java(JNIEnv *env, 
            jobject ctx_obj, TPQCTL *ctl_c)
{
    jobject ret = NULL;
    
out:

    return ret;
}

/* vim: set ts=4 sw=4 et smartindent: */
