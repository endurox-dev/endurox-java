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

#define OFSZ(s,e)   EXOFFSET(s,e), EXELEM_SIZE(s,e)

/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/

/**
 * Only field not mapped is ClientId cltid, this shall be driven manually
 */
exprivate exjobjmap_t M_fieldmap[] =
{    
    {"flags",       "J",                    OFSZ(TPQCTL,flags)},
    {"priority",    "J",                    OFSZ(TPQCTL,priority)},
    {"diagnostic",  "J",                    OFSZ(TPQCTL,diagnostic)},
    {"diagmsg",     "Ljava/lang/String;",   OFSZ(TPQCTL,diagmsg)},
    {"msgid",       "[B",                   OFSZ(TPQCTL,diagmsg)},
    {"corrid",      "[B",                   OFSZ(TPQCTL,corrid)},
    {"replyqueue",  "Ljava/lang/String;",   OFSZ(TPQCTL,replyqueue)},
    {"failurequeue","Ljava/lang/String;",   OFSZ(TPQCTL,failurequeue)},
    {"urcode",      "J",                    OFSZ(TPQCTL,urcode)},
    {"appkey",      "J",                    OFSZ(TPQCTL,appkey)},
    {"delivery_qos","J",                    OFSZ(TPQCTL,delivery_qos)},
    {"reply_qos",   "J",                    OFSZ(TPQCTL,reply_qos)},
    {"exp_time",    "J",                    OFSZ(TPQCTL,exp_time)},
    {NULL}
};

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
    
    jclass clz;
    jfieldID fid;
    jobject jcltid;

    clz = (*env)->FindClass(env, TPQCTL_CLASS);

    if (NULL==clz)
    {        
        /* I guess we need to abort here! */
        NDRX_LOG(log_error, "Failed to to get %s class!", TPQCTL_CLASS);
        ndrxj_atmi_throw(env, NULL, TPESYSTEM, "Failed get class [%s]", 
                    TPQCTL_CLASS);
        EXFAIL_OUT(ret);
    }
    
    /* Load values to C */
    
    if (EXSUCCEED!=ndrxj_cvt_to_c(env, 
            ctx_obj, M_fieldmap, clz, TPQCTL_CLASS,
            ctl_Java, ctl_c))
    {
        NDRX_LOG(log_error, "Failed to convert %s to TPQCTL!", TPQCTL_CLASS);
        EXFAIL_OUT(ret);
    }
    
    /* convert client id */
    if (NULL==(fid = (*env)->GetFieldID(env, clz, "cltid", "Ljava/lang/ClientId;")))
    {
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_NDRX, 
                "Failed to get [cltid] descr from QTPQCTL: %s");
        EXFAIL_OUT(ret);
    }
    
    /* get object field */
    jcltid = (*env)->GetObjectField(env, ctl_Java, fid);
    
    if (NULL==jcltid)
    {
        ndrxj_atmi_throw(env, NULL, TPEINVAL, "cltid is NULL in TPQCTL!");
        EXFAIL_OUT(ret);
    }
    
    /* convert to C */
    if (EXSUCCEED!=ndrxj_atmi_ClientId_translate_toc(env, 
        jcltid, &(ctl_c->cltid)))
    {
        NDRX_LOG(log_error, "Failed to convert client id");
        EXFAIL_OUT(ret);
    }
    
out:
       
    if (NULL!=clz)
    {
        (*env)->DeleteLocalRef( env, clz);
    }

    if (EXSUCCEED!=ret && !(*env)->ExceptionCheck(env))
    {
        ndrxj_atmi_throw(env, NULL, TPEINVAL, "Failed to convert TPQCTL to C "
                "from java - see logs!");
        EXFAIL_OUT(ret);
    }

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