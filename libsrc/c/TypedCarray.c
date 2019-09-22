/**
 * @brief TypedJson backings
 *
 * @file TypedCarray.c
 */
/* -----------------------------------------------------------------------------
 * Enduro/X Middleware Platform for Distributed Transaction Processing
 * Copyright (C) 2009-2016, ATR Baltic, Ltd. All Rights Reserved.
 * Copyright (C) 2017-2019, Mavimax, Ltd. All Rights Reserved.
 * This software is released under one of the following licenses:
 * LGPL or Mavimax's license for commercial use.
 * See LICENSE file for full text.
 *
 * C (as designed by Dennis Ritchie and later authors) language code is licensed
 * under Enduro/X Modified GNU Affero General Public License, version 3.
 * See LICENSE_C file for full text.
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
 * Set JSON string value.
 * Reallocate the JSON buffer if it is shorter than string value.
 * Update string object's length in java side to match the string just set
 * @param env java env
 * @param data TypedString object
 * @param s Sring value to set
 */
expublic JNIEXPORT void JNICALL ndrxj_Java_org_endurox_TypedCarray_setBytes
  (JNIEnv * env, jobject data, jbyteArray b)
{
    char *cdata;
    long clen;
    int ret = EXSUCCEED;
    int new_size, bufsz;
    jboolean n_carray_copy = EXFALSE;
    char * n_carray = NULL;
    
    /* Validate NULL string - not acceptable... */
    
    if (NULL==b)
    {
         ndrxj_atmi_throw(env, data, NULL, TPEINVAL, "byte array must not be NULL!");
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
    
    n_carray = (char*)(*env)->GetByteArrayElements(env, b, &n_carray_copy);
    new_size = (*env)->GetArrayLength(env, b);
    
    bufsz = tptypes(cdata, NULL, NULL);
    
    if (bufsz < 1)
    {
         ndrxj_atmi_throw(env, data, NULL, tperrno, tpstrerror(tperrno));
         EXFAIL_OUT(ret);
    }
    
    if (bufsz < new_size)
    {
        /* Reallocate buffer! */
        NDRX_LOG(log_debug, "Realloc carray buffer from %d to %d",
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
    else
    {
        /* just update the used length... data is the same... */
        if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_set_buffer(env, 
                data, cdata, new_size))
        {
            NDRX_LOG(log_error, "Failed to update buffer c address");
            EXFAIL_OUT(ret);
        }
    }
    
    memcpy(cdata, n_carray, new_size);
    
out:
    
    if(n_carray_copy)
    {
       (*env)->ReleaseByteArrayElements(env, b, (signed char *)n_carray, JNI_ABORT);
    }

    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);

    return;
}

/**
 * Read JSON value from C side buffer
 * @param env java env
 * @param data TypedString object
 * @return Java string
 */
expublic JNIEXPORT jbyteArray JNICALL ndrxj_Java_org_endurox_TypedCarray_getBytes
  (JNIEnv * env, jobject data)
{
    char *cdata;
    long clen;
    jbyteArray ret = NULL;
    
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
    
    ret = (*env)->NewByteArray(env, (jsize)clen);

    if (NULL==ret)
    {
        /* will it create exception??? */
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_ULOG, 
                "Failed to create byte array with: %s, size: %d", (int)clen);
        goto out;
    }

    (*env)->SetByteArrayRegion(env, ret, 0, clen, 
                            (jbyte*)cdata);

    /* Will it create exception? */
    if((*env)->ExceptionCheck(env))
    {
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_ULOG, 
                "Failed to set data bytes: %s");
        goto out;
    }
    
out:
    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);

    return ret;
}

/* vim: set ts=4 sw=4 et smartindent: */
