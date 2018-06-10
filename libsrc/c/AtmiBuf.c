/* 
** ATMI Context backing JNI functions
**
** @file AtmiCtx.c
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
#include "org_endurox_AtmiBuf.h"
#include <atmi.h>
#include <oatmi.h>
#include <ndebug.h>
#include "libsrc.h"
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/

/**
 * Free up the the context
 * @param env java env
 * @param obj ATMI buffer Object
 * @param cPtr C pointer to ATMI Buffer
 */
expublic void JNICALL Java_org_endurox_AtmiBuf_tpfree (JNIEnv *env, jobject obj, 
        jlong cPtr)
{
    TPCONTEXT_T ctx;

    jclass objClass = (*env)->GetObjectClass(env, obj);
    jfieldID atmi_ctx_fld = (*env)->GetFieldID(env, objClass, "ctx", "Lorg/endurox/AtmiCtx;");
    jobject atmi_ctx_obj = (*env)->GetObjectField(env, obj, atmi_ctx_fld);
    
    if (NULL==(ctx=ndrxj_get_ctx(env, atmi_ctx_obj, EXTRUE)))
    {
        goto out;
    }

    NDRX_LOG(log_debug, "About to free up: context: %p buf: %p",
            ctx, (void *)cPtr);
    
    tpfree((char *)cPtr);
    
    /* unset context */
    tpsetctxt(TPNULLCONTEXT, 0L);

out:
    /* return object */
    return;
}

/**
 * Translate ATMI buffer object to Java Typed object
 * @param env Java ENV
 * @param ctx_obj ATMI Context object
 * @param is_ctxset is ATMI context set arelady?
 * @param data data buffer
 * @param len data len
 * @param type optional data type
 * @param subtype optional sub-type
 * @return NULL (in case of error) java object
 */
expublic jobject ndrxj_atmi_AtmiBuf_translate(JNIEnv *env, 
            jobject ctx_obj, int is_ctxset, char *data, long len,
            char *type, char *subtype)
{
    jobject ret = NULL;
    jclass bclz;
    jmethodID mid;
    int we_set_ctx = EXFALSE;
    char clazz[256];
    
    char type_buf[XATMI_TYPE_LEN+1]={EXEOS};
    char subtype_buf[XATMI_SUBTYPE_LEN]={EXEOS};

    char *p_type = type;
    char *p_subtype = subtype;
    
    /* Set context if needed */
    if (!is_ctxset)
    {
        if (NULL==ndrxj_get_ctx(env, ctx_obj, EXTRUE))
        {
            goto out;
        }
        
        we_set_ctx = EXTRUE;
    }
    
    NDRX_LOG(log_debug, "Translating ATMI buffer: %p", data);
    
    /* request for  */
    if (NULL==p_type)
    {
        p_type = type_buf;
        p_subtype = subtype_buf;
    
        /* request buffer type */
        if (NULL==data)
        {
            NDRX_STRCPY_SAFE(type_buf, "NULL");
        }
        else if (EXFAIL==tptypes(data, p_type, p_subtype))
        {
            NDRX_LOG(log_error, "Failed to get type for buffer: %s", 
                    tpstrerror(tperrno));
            
            /* throw exception */
            ndrxj_atmi_throw(env, tperrno, tpstrerror(tperrno));
            
            ret = NULL;
            goto out;
        }
    }
    
    /* allocate the object here according to the buffer type */
    
    if (0==strcmp(type, "NULL"))
    {
        /* NULL buffer object */
        snprintf(clazz, sizeof(clazz), "org/endurox/TypedNull");
    }
    else if (0==strncmp(type, "UBF", 3) || 
            0==strncmp(type, "FML", 3))
    {
        /* UBF object */
        snprintf(clazz, sizeof(clazz), "org/endurox/TypedUbf");
    }
    else if (0==strcmp(type, "CARRAY"))
    {
        /* Carray object */
        snprintf(clazz, sizeof(clazz), "org/endurox/TypedCarray");
    }
    else if (0==strcmp(type, "STRING"))
    {
        /* String object */
        snprintf(clazz, sizeof(clazz), "org/endurox/TypedString");
    }
    else if (0==strcmp(type, "VIEW"))
    {
        /* VIEW object */
        snprintf(clazz, sizeof(clazz), "org/endurox/TypedView");
    }
    else if (0==strcmp(type, "JSON"))
    {
        /* JSON object */
        snprintf(clazz, sizeof(clazz), "org/endurox/TypedJson");
    }
    else if (0==strcmp(type, "TPINIT"))
    {
        /* Init buffer */
        snprintf(clazz, sizeof(clazz), "org/endurox/TypedTpInit");
    }
    else
    {
        ndrxj_atmi_throw(env, TPEINVAL, "buffer type [%s] not supported", 
                type);
        goto out;
    }
    
    NDRX_LOG(log_debug, "Allocating [%s] class", clazz);
    
    bclz = (*env)->FindClass(env, clazz);
    
    if (NULL==bclz)
    {        
        NDRX_LOG(log_error, "Failed to find class [%s]", clazz);
        goto out;
        
    }
    
    /* create buffer object... */
    mid = (*env)->GetMethodID(env, bclz, "<init>", "(Lorg/endurox/AtmiCtx;ZJJ)V");
    
    if (NULL==mid)
    {
        NDRX_LOG(log_error, "Cannot get buffer constructor!");
        goto out;
    }

    NDRX_LOG(log_debug, "About to NewObject(%s)", clazz);
    
    ret = (*env)->NewObject(env, bclz, mid, ctx_obj, JNI_TRUE, (jlong)data, len);
    
    if (NULL==ret)
    {
        NDRX_LOG(log_error, "Failed to create [%s]", clazz);
        goto out;
    }
    
    NDRX_LOG(log_debug, "NewObject() done");
    

out:
    
    if (we_set_ctx)
    {
        /* return back to NULL */
        tpsetctxt(TPNULLCONTEXT, 0L);
    }

    return ret;
}

/**
 * Extract C data buffer from AtmiBuf object
 * CONTEXT: Assume it is set.
 * @param env Java env
 * @param data ATMI buffer
 * @param buf return pointer to data block
 * @param len return value of buffer len (if applicable)
 * @return EXSUCCEED in case of OK, EXFAIL in case of failure
 * @thorws java exceptions if any
 */
expublic int ndrxj_atmi_AtmiBuf_get_buffer(JNIEnv *env, 
            jobject data, char **buf, long *len)
{
    int ret = EXSUCCEED;
    
    jclass clz;
    jmethodID mid;
    
#if 0
    
    /* TODO:!!! */
    
    jlong cPtr; = (*env)->GetFieldID(env, cls, "iVal", "I");
    jlong cLen = (*env)->GetIntField(env, objarg, fidInt);
    printf("iVal: %d\n", iVal);
    
    
    clz = (*env)->FindClass(env, "org/endurox/AtmiBuf");

    if (NULL==clz)
    {        
        /* I guess we need to abort here! */
        NDRX_LOG(log_error, "Failed to get Atmi buffer class!");
        EXFAIL_OUT(ret);
    }

    /* create buffer object... */
    mid = (*M_srv_ctx_env)->GetMethodID(M_srv_ctx_env, bclz, 
            "tpCallDispatch", "(Lorg/endurox/TpSvcInfo;)V");

    if (NULL==mid)
    {
        NDRX_LOG(log_error, "Cannot get call dispatcher method at C side!");
        abort();
    }
    
#endif

    
out:
    
    return ret;
    
}

/* vim: set ts=4 sw=4 et cindent: */

