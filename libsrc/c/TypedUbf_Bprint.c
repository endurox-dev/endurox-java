/**
 * @brief Java UBF Backing routines
 *
 * @file TypedUbf_Bprint.c
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
 * Print the UBF buffer to STDOUT
 * @param env java env
 * @param data TypedUbf object
 */
expublic void JNICALL ndrxj_Java_org_endurox_TypedUbf_Bprint(JNIEnv *env, jobject data)
{
    char *cdata;
    long clen;
    
    /* get the context, switch */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
       return; 
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, &clen,
            NULL, EXFALSE, EXFALSE))
    {
        UBF_LOG(log_error, "Failed to get buffer data");
        goto out;
    }
    
    if (EXSUCCEED!=Bprint((UBFH *)cdata))
    {
        UBF_LOG(log_error, "%s: failed to Bprint %p buffer: %s", 
                __func__, cdata, Bstrerror(Berror));
        ndrxj_ubf_throw(env, Berror, "%s: failed to Bprint %p buffer: %s", 
                __func__, cdata, Bstrerror(Berror));
        goto out;
    }
    
out:
    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);
}

/**
 * Control data for compiled boolean expression printing
 */
typedef struct Bextread_ctl Bextread_ctl_t;
struct Bextread_ctl
{
    JNIEnv * env;
    jobject data;
    jobject reader;
};

/**
 * Read data line from BufferedReader
 * @param buffer output buffer
 * @param bufsz output buffer size
 * @param dataptr1 user specific data ptr, holds java context
 * @return number bytes read
 */
exprivate long Bextread_readf(char *buffer, long bufsz, void *dataptr1)
{
    Bextread_ctl_t *ctl = (Bextread_ctl_t *)dataptr1;
    long ret = EXSUCCEED;
    TPCONTEXT_T context;
    jstring str;
    jboolean n_str_copy = EXFALSE;
    const char *n_str;
    
    /* suspend ATMI context as java might perform some other actions
     * on given thread.
     */
    tpgetctxt(&context, 0L);
    /* Call server object */
    str = (*(ctl->env))->CallObjectMethod(ctl->env, ctl->reader, 
            ndrxj_clazz_BufferedReader_mid_readLine);
    /* restore ATMI context */
    tpsetctxt(context, 0L);
    
    if (NULL==str)
    {
        if((*(ctl->env))->ExceptionCheck(ctl->env))
        {
            
            NDRXJ_LOG_EXCEPTION((ctl->env), log_error, NDRXJ_LOGEX_UBF, 
                    "Failed to buffered read: %s");
            
            EXFAIL_OUT(ret);
        }
        
        UBF_LOG(log_debug, "EOF reached on buffered reader");
        ret = 0;
        goto out;
    }
    
    /* transfer bytes to buffer */
    n_str= (*(ctl->env))->GetStringUTFChars(ctl->env, str, &n_str_copy);
     
#ifdef NDRX_STRCPY_SAFE_DST
    NDRX_STRCPY_SAFE_DST(buffer, n_str, bufsz);
#else
    NDRX_STRNCPY_SAFE(buffer, n_str, bufsz);
#endif
    
    if (n_str_copy)
    {
        (*(ctl->env))->ReleaseStringUTFChars(ctl->env, str, n_str);
    }
    
    ret = strlen(buffer)+1;

out:

    return ret;
}

/**
 * Read UBF buffer from java input stream
 * @param env Java env
 * @param data UBF buffer
 * @param reader buffered input reader
 */
expublic void JNICALL ndrxj_Java_org_endurox_TypedUbf_Bextread
  (JNIEnv * env, jobject data, jobject reader)
{
    char *cdata;
    long clen;
    Bextread_ctl_t ctl;
    
    /* get the context, switch */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
       return; 
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, &clen,
        NULL, EXFALSE, EXFALSE))
    {
        UBF_LOG(log_error, "Failed to get buffer data");
        goto out;
    }
    
    /* read the buffer from interface */
    ctl.env = env;
    ctl.data = data;
    ctl.reader = reader;

    if (EXSUCCEED!=Bextreadcb((UBFH *)cdata, Bextread_readf, (void *)&ctl))
    {
        UBF_LOG(log_error, "Failed to call Bextreadcb(): %s", Bstrerror(Berror));
        ndrxj_ubf_throw(env, Berror, "Failed to call Bextreadcb(): %s", 
                Bstrerror(Berror));
        goto out;
    }
    
out:
    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);
}

/**
 * Test data holder for bfprintcb_data
 */
typedef struct rw_data rw_data_t;
struct rw_data
{
    long offset; /**< current offset in UBF */
    char *buf; /**< currently allocate buffer (can realloc to extend)  */
    long size;  /**< array size */
    JNIEnv * env; /**< for exception generating */
};


/**
 * Callback for read function. In first call the full bufz size must be
 * read. For second call the number of bytes we have left in buffer must be read.
 * @param buffer buffer to put data into
 * @param bufsz requested size (for first call full bytes must be read)
 * @param dataptr1 user ptr
 * @return  number of bytes written to buffer
 */
exprivate long Bread_readf(char *buffer, long bufsz, void *dataptr1)
{
    long ret = EXFAIL;
    rw_data_t *ctl = (rw_data_t *)dataptr1;
    
    if (0==ctl->offset)
    {
        if (ctl->size < bufsz)
        {
            /*  */
            UBF_LOG(log_error, "ERROR ! Requested %ld bytes, "
                    "but our array has only %ld",
                    bufsz, ctl->size);
            
            /* generate exception */
            ndrxj_ubf_throw(ctl->env, BEINVAL, "ERROR ! Requested %ld bytes, "
                    "but our array has only %ld", bufsz, ctl->size);
            EXFAIL_OUT(ret);
        }
        
        memcpy(buffer, ctl->buf, bufsz);
        ctl->offset+=bufsz;
        ret = bufsz;
    }
    else
    {
        ret = ctl->size - ctl->offset;
        /* just copy off what we have left here */
        memcpy(buffer, ctl->buf+ctl->offset, ret);
    }
    
out:
    return ret;
}

/**
 * Read UBF buffer from byte array
 * @param env java env
 * @param data UBF buffer
 * @param jb platform specific UBF bytes to read from
 */
expublic void JNICALL ndrxj_Java_org_endurox_TypedUbf_Bread
  (JNIEnv * env, jobject data, jbyteArray jb)
{
    rw_data_t ctl;
    char *cdata;
    long clen;
    jboolean n_array_copy;
    char * n_array = (char*)(*env)->GetByteArrayElements(env, jb, &n_array_copy);
    
    /* get the context, switch */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
       return; 
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, &clen,
        NULL, EXFALSE, EXFALSE))
    {
        UBF_LOG(log_error, "Failed to get buffer data");
        goto out;
    }
    
    /* read the buffer from interface */
    
    memset(&ctl, 0, sizeof(ctl));
    
    ctl.env = env;
    ctl.buf = n_array;
    ctl.size = (*env)->GetArrayLength(env, jb);
    
    NDRX_LOG(log_debug, "Reading buffer len: %ld", (long)ctl.size);
    
    if (EXSUCCEED!=Breadcb ((UBFH *)cdata, Bread_readf, (void *)&ctl))
    {
        UBF_LOG(log_error, "Failed to call Breadcb(): %s", Bstrerror(Berror));
        ndrxj_ubf_throw(env, Berror, "Failed to call Breadcb(): %s", 
                Bstrerror(Berror));
        goto out;
    }
    
out:
    
    if(n_array_copy)
    {
       (*env)->ReleaseByteArrayElements(env, jb, (signed char *)n_array, JNI_ABORT);
    }

    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);
}

/**
 * Realloc/Fill the memory block callback
 * @param buffer data to write
 * @param bufsz data size
 * @param dataptr1 user ptr
 * @return number of bytes written, must match bufsz in case of succeed
 */
exprivate long Bwrite_writef(char *buffer, long bufsz, void *dataptr1)
{
    long ret = EXFAIL;
    rw_data_t *ctl = (rw_data_t *)dataptr1;
    
    if (NULL==ctl->buf)
    {
        if (NULL==(ctl->buf = NDRX_MALLOC(bufsz)))
        {
            int err = errno;
            UBF_LOG(log_error, "Failed to malloc %ld bytes: %s", 
                    bufsz, strerror(err));
            userlog("Failed to malloc %ld bytes: %s", 
                    bufsz, strerror(err));
            
            ndrxj_ubf_throw(ctl->env, BEUNIX, "%s: Failed to malloc %ld bytes: %s", 
                    bufsz, strerror(err));
            
            EXFAIL_OUT(ret);
        }
    }
    else 
    {
        long newsz = ctl->offset+bufsz;
        if (NULL==(ctl->buf = NDRX_REALLOC(ctl->buf, newsz)))
        {
            int err = errno;
            UBF_LOG(log_error, "Failed to realloc %ld bytes: %s", 
                    newsz, strerror(err));
            userlog("Failed to realloc %ld bytes: %s", 
                    newsz, strerror(err));
            
             ndrxj_ubf_throw(ctl->env, BEUNIX, "%s: Failed to realloc %ld bytes: %s", 
                    newsz, strerror(err));
            
            EXFAIL_OUT(ret);
        }
    }
    
    memcpy(ctl->buf+ctl->offset, buffer, bufsz);
    
    ctl->offset+=bufsz;
    ret = bufsz;
    
out:
    return ret;
}

/**
 * Write UBF buffer to byte array
 * @param env java env
 * @param data UBF buffer
 * @return byte array of the serialized platform specific UBF 
 */
expublic jbyteArray JNICALL ndrxj_Java_org_endurox_TypedUbf_Bwrite
  (JNIEnv * env, jobject data)
{
    rw_data_t ctl;
    char *cdata;
    long clen;
    jbyteArray ret = NULL;
    
    memset(&ctl, 0, sizeof(ctl));
    
    ctl.env = env;
    /* get the context, switch */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
       return NULL; 
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, &clen,
        NULL, EXFALSE, EXFALSE))
    {
        UBF_LOG(log_error, "Failed to get buffer data");
        goto out;
    }
    
    if (EXSUCCEED!=Bwritecb ((UBFH *)cdata, Bwrite_writef, (void *)&ctl))
    {
        UBF_LOG(log_error, "Failed to call Bwritecb(): %s", Bstrerror(Berror));
        ndrxj_ubf_throw(env, Berror, "Failed to call Bwritecb(): %s", 
                Bstrerror(Berror));
        goto out;
    }
    
    /* now we shall return java byte array */
    
    ret = (*env)->NewByteArray(env, (jsize)ctl.offset);

    if (NULL==ret)
    {
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_ULOG, 
                "Failed to create byte array with: %s, size: %ld", 
                (long)ctl.offset);
        goto out;
    }
    
    (*env)->SetByteArrayRegion(env, ret, 0, ctl.offset, 
			(jbyte*)ctl.buf);
    
    if((*env)->ExceptionCheck(env))
    {
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_ULOG, 
                "Failed to set data bytes: %s");
        goto out;
    }
    
    
out:
    
    if (NULL!=ctl.buf)
    {
        NDRX_FREE(ctl.buf);
    }
    /* switch context back */
    tpsetctxt(TPNULLCONTEXT, 0L);

    return ret;
}

/* vim: set ts=4 sw=4 et smartindent: */
