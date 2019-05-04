/**
 * @brief TPTRANID structure translate from/to Java/C
 *
 * @file TPTRANID.c
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
#define TPTRANID_CLASS     "org/endurox/TPTRANID"

#define OFSZ(s,e)   EXOFFSET(s,e), EXELEM_SIZE(s,e)

/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/

/**
 * Copy C tid to java byte array and vice versa
 */
exprivate exjobjmap_t M_fieldmap[] =
{    
    {"data",       "[B",                    OFSZ(TPTRANID_conv,tid)},
    {NULL}
};

/*---------------------------Prototypes---------------------------------*/

/**
 * Translate TPTRANID to C from java
 * @param env java env
 * @param ctx_obj ATMI Context
 * @param tid_Java Java object
 * @param tid_c C object
 * @return EXSUCCEED/EXFAIL
 */
expublic int ndrxj_atmi_TPTRANID_translate2c(JNIEnv *env, 
            jobject ctx_obj, jobject tid_Java, TPTRANID_conv *tid_c)
{
    int ret = EXSUCCEED;
    jclass clz;

    clz = (*env)->FindClass(env, TPTRANID_CLASS);

    if (NULL==clz)
    {        
        /* I guess we need to abort here! */
        NDRX_LOG(log_error, "Failed to to get %s class!", TPTRANID_CLASS);
        ndrxj_atmi_throw(env, NULL, NULL, TPESYSTEM, "Failed get class [%s]", 
                    TPTRANID_CLASS);
        EXFAIL_OUT(ret);
    }
    
    /* Load values to C */
    
    if (EXSUCCEED!=ndrxj_cvt_to_c(env, 
            ctx_obj, M_fieldmap, clz, TPTRANID_CLASS,
            tid_Java, tid_c))
    {
        NDRX_LOG(log_error, "Failed to convert %s to TPTRANID!", TPTRANID_CLASS);
        EXFAIL_OUT(ret);
    }
    
out:
       
    if (NULL!=clz)
    {
        (*env)->DeleteLocalRef( env, clz);
    }

    if (EXSUCCEED!=ret && !(*env)->ExceptionCheck(env))
    {
        ndrxj_atmi_throw(env, NULL, NULL, TPEINVAL, "Failed to convert TPTRANID to C "
                "from java - see logs!");
    }

    return ret;
}

/**
 * Translate C struct to Java
 * @param env java env
 * @param ctx_obj ATMI Ctx obj
 * @param tid_Java java transaction id (optional)
 * @param tid_c C Control struct
 * @return EXSUCCEED/EXFAIL
 */
expublic jobject ndrxj_atmi_TPTRANID_translate2java(JNIEnv *env, 
            jobject ctx_obj, jobject tid_Java, TPTRANID_conv *tid_c)
{
    int ret = EXSUCCEED;
    jobject retObj = NULL;
    jclass clz;
    jmethodID mid;

    clz = (*env)->FindClass(env, TPTRANID_CLASS);

    if (NULL==clz)
    {        
        /* I guess we need to abort here! */
        NDRX_LOG(log_error, "Failed to to get %s class!", TPTRANID_CLASS);
        ndrxj_atmi_throw(env, NULL, NULL, TPESYSTEM, "Failed get class [%s]", 
                    TPTRANID_CLASS);
        EXFAIL_OUT(ret);
    }


    if (NULL==tid_Java)
    {
        /* Allocate java object */
            /* create buffer object... */
        mid = (*env)->GetMethodID(env, clz, "<init>", "()V");

        if (NULL==mid)
        {
            NDRX_LOG(log_error, "Cannot get buffer constructor!");
            EXFAIL_OUT(ret);
        }

        NDRX_LOG(log_debug, "About to NewObject(%s)", TPTRANID_CLASS);

        retObj = (*env)->NewObject(env, clz, mid);
    }
    else
    {
        NDRX_LOG(log_debug, "Re-use existing java qtid");
        retObj = tid_Java;
    }
    
    /* Load values to C */
    
    if (EXSUCCEED!=ndrxj_cvt_to_java(env, 
            ctx_obj, M_fieldmap, clz, TPTRANID_CLASS,
            tid_c, retObj))
    {
        NDRX_LOG(log_error, "Failed to convert C TPTRANID to java %s!", TPTRANID_CLASS);
        EXFAIL_OUT(ret);
    }
    
out:
       
    if (NULL!=clz)
    {
        (*env)->DeleteLocalRef( env, clz);
    }

    if (EXSUCCEED!=ret && NULL!=retObj && NULL==tid_Java)
    {
        (*env)->DeleteLocalRef( env, retObj);
    }

    if (EXSUCCEED!=ret && !(*env)->ExceptionCheck(env))
    {
        ndrxj_atmi_throw(env, NULL, NULL, TPEINVAL, "Failed to convert C TPTRANID to java "
                "- see logs!");
    }

    return retObj;
}

/* vim: set ts=4 sw=4 et smartindent: */
