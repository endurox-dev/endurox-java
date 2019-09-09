/**
 * @brief TPQCTL structure translate from/to Java/C
 *
 * @file TPQCTL.c
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
    {&ndrxj_clazz_TPQCTL_fid_flags,     "flags",       "J",                 OFSZ(TPQCTL,flags)},
    {&ndrxj_clazz_TPQCTL_fid_priority,  "priority",    "J",                 OFSZ(TPQCTL,priority)},
    {&ndrxj_clazz_TPQCTL_fid_diagnostic,"diagnostic",  "J",                 OFSZ(TPQCTL,diagnostic)},
    {&ndrxj_clazz_TPQCTL_fid_diagmsg,   "diagmsg",     "Ljava/lang/String;",OFSZ(TPQCTL,diagmsg)},
    {&ndrxj_clazz_TPQCTL_fid_msgid,     "msgid",       "[B",                OFSZ(TPQCTL,msgid)},
    {&ndrxj_clazz_TPQCTL_fid_corrid,    "corrid",      "[B",                OFSZ(TPQCTL,corrid)},
    {&ndrxj_clazz_TPQCTL_fid_replyqueue, "replyqueue",  "Ljava/lang/String;",OFSZ(TPQCTL,replyqueue)},
    {&ndrxj_clazz_TPQCTL_fid_failurequeue, "failurequeue","Ljava/lang/String;",OFSZ(TPQCTL,failurequeue)},
    {&ndrxj_clazz_TPQCTL_fid_urcode,    "urcode",      "J",                 OFSZ(TPQCTL,urcode)},
    {&ndrxj_clazz_TPQCTL_fid_appkey,    "appkey",      "J",                 OFSZ(TPQCTL,appkey)},
    {&ndrxj_clazz_TPQCTL_fid_delivery_qos, "delivery_qos","J",              OFSZ(TPQCTL,delivery_qos)},
    {&ndrxj_clazz_TPQCTL_fid_reply_qos, "reply_qos",   "J",                 OFSZ(TPQCTL,reply_qos)},
    {&ndrxj_clazz_TPQCTL_fid_exp_time,  "exp_time",    "J",                 OFSZ(TPQCTL,exp_time)},
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
    
    jobject jcltid;
    
    /* Load values to C */
    
    if (EXSUCCEED!=ndrxj_cvt_to_c(env, 
            ctx_obj, M_fieldmap, TPQCTL_CLASS,
            ctl_Java, ctl_c))
    {
        NDRX_LOG(log_error, "Failed to convert %s to TPQCTL!", TPQCTL_CLASS);
        EXFAIL_OUT(ret);
    }
    
    /* get object field */
    jcltid = (*env)->GetObjectField(env, ctl_Java, ndrxj_clazz_TPQCTL_fid_cltid);
    
    if (NULL!=jcltid)
    {
        ndrxj_atmi_throw(env, NULL, NULL, TPEINVAL, "cltid is NULL in TPQCTL!");
        EXFAIL_OUT(ret);

        /* convert to C */
        if (EXSUCCEED!=ndrxj_atmi_ClientId_translate_toc(env, 
            jcltid, &(ctl_c->cltid)))
        {
            NDRX_LOG(log_error, "Failed to convert client id");
            EXFAIL_OUT(ret);
        }
    }
    else
    {
        ctl_c->cltid.clientdata[0] = EXEOS;
    }
    
out:

    if (EXSUCCEED!=ret && !(*env)->ExceptionCheck(env))
    {
        ndrxj_atmi_throw(env, NULL, NULL, TPEINVAL, "Failed to convert TPQCTL to C "
                "from java - see logs!");
    }

    return ret;
}

/**
 * Translate C struct to Java
 * @param env java env
 * @param ctx_obj ATMI Ctx obj
 * @param ctl_Java optional java control struct (use this, if passed)
 * @param ctl_c C Control struct
 * @return EXSUCCEED/EXFAIL
 */
expublic jobject ndrxj_atmi_TPQCTL_translate2java(JNIEnv *env, 
            jobject ctx_obj, jobject ctl_Java, TPQCTL *ctl_c)
{
    int ret = EXSUCCEED;
    jobject retObj = NULL;
    jobject jcltid;

    if (NULL==ctl_Java)
    {
        /* Allocate java object */
        
        NDRX_LOG(log_debug, "About to NewObject(%s)", TPQCTL_CLASS);

        retObj = (*env)->NewObject(env, ndrxj_clazz_TPQCTL, ndrxj_clazz_TPQCTL_mid_INIT);
    }
    else
    {
        NDRX_LOG(log_debug, "Re-use existing java qctl");
        retObj = ctl_Java;
    }
    
    /* Load values to C */
    
    if (EXSUCCEED!=ndrxj_cvt_to_java(env, 
            ctx_obj, M_fieldmap, TPQCTL_CLASS,
            ctl_c, retObj))
    {
        NDRX_LOG(log_error, "Failed to convert C TPQCTL to java %s!", TPQCTL_CLASS);
        EXFAIL_OUT(ret);
    }
    
    /* convert to java */
    if (NULL==(jcltid = ndrxj_atmi_ClientId_translate(env, ctx_obj, EXTRUE,
        &(ctl_c->cltid))))
    {
        NDRX_LOG(log_error, "Failed to convert client id");
        EXFAIL_OUT(ret);
    }
    
    /* set field */
    
    (*env)->SetObjectField(env, retObj, ndrxj_clazz_TPQCTL_fid_cltid, jcltid);
    
out:

    if (NULL!=retObj && EXSUCCEED!=ret && NULL==ctl_Java)
    {
        (*env)->DeleteLocalRef( env, retObj);
    }

    if (EXSUCCEED!=ret && !(*env)->ExceptionCheck(env))
    {
        ndrxj_atmi_throw(env, NULL, NULL, TPEINVAL, "Failed to convert C TPQCTL to java "
                "- see logs!");
    }

    return retObj;
}

/* vim: set ts=4 sw=4 et smartindent: */
