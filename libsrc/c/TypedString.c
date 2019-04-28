/**
 * @brief TypedString backings
 *
 * @file TypedString.c
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
#include "nerror.h"
#include <ndrstandard.h>
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/

/**
 * Set string value.
 * Reallocate the STRING buffer if it is shorter than string value.
 * Update string object's length in java side to match the string just set
 * @param env java env
 * @param data TypedString object
 * @param s Sring value to set
 */
expublic void JNICALL ndrxj_Java_org_endurox_TypedString_setString
  (JNIEnv * env, jobject data, jstring s)
{
    char *cdata;
    long clen;
    int ret = EXSUCCEED;
    jboolean n_str_copy = EXFALSE;
    const char *n_str;
    int new_size, bufsz;
    
    /* Validate NULL string - not acceptable... */
    
    if (NULL==s)
    {
         ndrxj_atmi_throw(env, data, NULL, TPEINVAL, "string must not be NULL!");
         return; /* <<<< RETURN !!! */
    }
    
    /* get the context, switch */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
        EXFAIL_OUT(ret);
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, &clen,
            NULL, EXFALSE, EXFALSE))
    {
        NDRX_LOG(log_error, "Failed to get buffer data");
        EXFAIL_OUT(ret);
    }
    
    /* get buffer size & compare with "S" string length
     * if fits in fine, if not - realloc to bigger
     */
    
    /* Get string */
    n_str = (*env)->GetStringUTFChars(env, s, &n_str_copy);
    new_size = strlen(n_str) + 1;
    
    bufsz = tptypes(cdata, NULL, NULL);
    
    if (bufsz < 1)
    {
         ndrxj_atmi_throw(env, data, NULL, tperrno, tpstrerror(tperrno));
         EXFAIL_OUT(ret);
    }
    
    if (bufsz < new_size)
    {
        /* Reallocate buffer! */
        NDRX_LOG(log_debug, "Realloc string buffer from %d to %d",
            bufsz, new_size);
        
        if (NULL==(cdata = tprealloc(cdata, new_size)))
        {
            /* Set buffer to NULL! to avoid crash at free */
            if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_set_buffer(env, 
                    data, NULL, 0L))
            {
                NDRX_LOG(log_error, "Failed to reset buffer to NULL");
            }
            
            /* throw exception */
            ndrxj_atmi_throw(env, data, NULL, tperrno, tpstrerror(tperrno));
            EXFAIL_OUT(ret);
        }
        else
        {
            /* Set new ptr */
            if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_set_buffer(env, 
                    data, cdata, new_size))
            {
                NDRX_LOG(log_error, "Failed to update buffer c address");
                EXFAIL_OUT(ret);
            }
        }
    }
    
    strcpy(cdata, n_str);

out:
    
    if (n_str_copy)
    {
        (*env)->ReleaseStringUTFChars(env, s, n_str);
    }

    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);

    return;
}

/**
 * Read string value from C side buffer
 * @param env java env
 * @param data TypedString object
 * @return Java string
 */
expublic jstring JNICALL ndrxj_Java_org_endurox_TypedString_getString
  (JNIEnv * env, jobject data)
{
    char *cdata;
    long clen;
    jstring ret = NULL;
    
    /* get the context, switch */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
        goto out;
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, &clen,
            NULL, EXFALSE, EXFALSE))
    {
        NDRX_LOG(log_error, "Failed to get buffer data");
        goto out;
    }
    
    
    ret = (*env)->NewStringUTF(env, cdata);
    
out:
    
    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);

    return ret;
}

/* vim: set ts=4 sw=4 et smartindent: */
