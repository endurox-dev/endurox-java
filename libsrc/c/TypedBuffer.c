/**
 * @brief Typed buffer operations
 *   TODO: We shall agree here how to handle exception?
 *   Check them only at the level of JNI calls? Or we can do this inside too?
 *
 * @file TypedBuffer.c
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
#include <jni.h>
#include <exjglue.h>
#include <atmi.h>
#include <oatmi.h>
#include <ndebug.h>
#include "libsrc.h"
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
#define TYPEDBUFFER_CLASS     "org/endurox/TypedBuffer"
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/

/**
 * Transfer finalize flag from two buffers.
 * This assumes that ATMI context is set.
 * @param[in] env java env
 * @param[in out] to_data to data object
 * @param[in out] from_data from data object
 * @param[in] from_invalidate remove c ptr from "from" buffer
 * @return EXSUCCEED/EXFAIL
 */
expublic int ndrxj_TypedBuffer_finalize_transfer(JNIEnv *env, 
        jobject to_data, jobject from_data, int from_invalidate)
{
    int ret = EXSUCCEED;

    jboolean doFinalize;
 
    
    doFinalize = (*env)->GetBooleanField(env, from_data, 
            ndrxj_clazz_TypedBuffer_fid_doFinalize);
    
    /* now set the field to dest buffer */
    NDRX_LOG(log_debug, "transfer of auto flag: %d", (int)doFinalize);
    (*env)->SetBooleanField(env, to_data, ndrxj_clazz_TypedBuffer_fid_doFinalize, 
            doFinalize);
    
    /* now set org buffer to false */
    (*env)->SetBooleanField(env, from_data, ndrxj_clazz_TypedBuffer_fid_doFinalize, 
            JNI_FALSE);
    
    if (from_invalidate)
    {
        
        (*env)->SetLongField(env, from_data, ndrxj_clazz_TypedBuffer_fid_cPtr, 0L);
    }
    
    if((*env)->ExceptionCheck(env))
    {
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_NDRX, 
                "Failed to transfer doFinalize flag between buffers: %s - %p %p",
                from_data, to_data);
        EXFAIL_OUT(ret);
    }
    
out:
    
    return ret;
}

/**
 * Get ATMI Context from ATMI buffer
 * @param env java env
 * @param atmiBufObj ATMI buffer object
 * @param do_set should we set the context?
 * @return context pointer
 */
expublic TPCONTEXT_T ndrxj_TypedBuffer_get_ctx(JNIEnv *env, 
        jobject atmiBufObj, int do_set)
{
    TPCONTEXT_T ctx = NULL;
    
    jobject atmi_ctx_obj = (*env)->GetObjectField(env, atmiBufObj, 
            ndrxj_clazz_TypedBuffer_fid_ctx);
    
    if (NULL==atmi_ctx_obj)
    {
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_ULOG, 
                "Context (ctx) is NULL for TypedBuffer: %s");
        goto out;
    }
    
    if (NULL==(ctx=ndrxj_get_ctx(env, atmi_ctx_obj, do_set)))
    {
        goto out;
    }
    
out:
    
    return ctx;
}

/**
 * Free up the the context
 * @param env java env
 * @param obj ATMI buffer Object
 * @param cPtr C pointer to ATMI Buffer
 */
expublic void JNICALL ndrxj_Java_org_endurox_TypedBuffer_tpfree (JNIEnv *env, jobject obj, 
        jlong cPtr)
{
    TPCONTEXT_T ctx;

    /* WARNING !!!!
     * 
     * we get some problems with finalizers - thus better allocate new context
     * Looks like java is killing AtmiContexts in the same time with the buffer
     * free. And the AtmiContext is no more accessible 
     * !!!! WARNING
     * 
     *
    jclass objClass = (*env)->GetObjectClass(env, obj);
    jfieldID atmi_ctx_fld = (*env)->GetFieldID(env, objClass, "ctx", 
            "Lorg/endurox/TypedBuffer;");
    jobject atmi_ctx_obj = (*env)->GetObjectField(env, obj, atmi_ctx_fld);
    
    if (NULL==(ctx=ndrxj_get_ctx(env, atmi_ctx_obj, EXTRUE)))
    {
        goto out;
    }
*/
    
    ctx = tpnewctxt(EXFALSE, EXTRUE);
    
    NDRX_LOG(log_debug, "About to free up buf: %p",
            (void *)(long)cPtr);
    
    tpfree((char *)(long)cPtr);
    
    /* unset context */
    tpsetctxt(TPNULLCONTEXT, 0L);
    
    tpfreectxt(ctx);
    
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
 * @param finalize EXTRUE if buffer shall be finalized,
 *    EXFALSE, if buffer shall not be finalized (auto or part of other buf)
 * @return NULL (in case of error) java object
 */
expublic jobject ndrxj_atmi_TypedBuffer_translate(JNIEnv *env, 
            jobject ctx_obj, int is_ctxset, char *data, long len,
            char *type, char *subtype, int finalize)
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
            
            /* throw exception - format is required as if message will contain
             * some format symbols, then we get corrupted output.*/
            ndrxj_atmi_throw(env, NULL, NULL, tperrno, "%s", tpstrerror(tperrno));
            
            ret = NULL;
            goto out;
        }
    }
    
    /* allocate the object here according to the buffer type */
    
    NDRX_LOG(log_debug, "Translating ATMI buffer: %p, type: [%s]", data,
                        p_type);
    
    if (0==strcmp(p_type, "NULL"))
    {
        /* NULL buffer object 
        snprintf(clazz, sizeof(clazz), "org/endurox/TypedNull");
         * ??? maybe just return NULL */ 
        
        /* NULL buffer... nothing todo... */
        goto out;
    }
    else if (0==strncmp(p_type, "UBF", 3) || 
            0==strncmp(p_type, "FML", 3))
    {
        /* UBF object */
        snprintf(clazz, sizeof(clazz), "org/endurox/TypedUbf");
        bclz = ndrxj_clazz_TypedUbf;
        mid = ndrxj_clazz_TypedUbf_mid_INIT;
    }
    else if (0==strcmp(p_type, "CARRAY"))
    {
        /* Carray object */
        snprintf(clazz, sizeof(clazz), "org/endurox/TypedCarray");
        bclz = ndrxj_clazz_TypedCarray;
        mid = ndrxj_clazz_TypedCarray_mid_INIT;
    }
    else if (0==strcmp(p_type, "STRING"))
    {
        /* String object */
        snprintf(clazz, sizeof(clazz), "org/endurox/TypedString");
        bclz = ndrxj_clazz_TypedString;
        mid = ndrxj_clazz_TypedString_mid_INIT;
    }
    else if (0==strcmp(p_type, "VIEW"))
    {
        /* VIEW object */
        snprintf(clazz, sizeof(clazz), "org/endurox/TypedView");
        bclz = ndrxj_clazz_TypedView;
        mid = ndrxj_clazz_TypedView_mid_INIT;
    }
    else if (0==strcmp(p_type, "JSON"))
    {
        /* JSON object */
        snprintf(clazz, sizeof(clazz), "org/endurox/TypedJson");
        bclz = ndrxj_clazz_TypedJson;
        mid = ndrxj_clazz_TypedJson_mid_INIT;
    }
    else if (0==strcmp(p_type, "TPINIT"))
    {
        /* Init buffer */
        snprintf(clazz, sizeof(clazz), "org/endurox/TypedTpInit");
        bclz = ndrxj_clazz_TypedTpInit;
        mid = ndrxj_clazz_TypedTpInit_mid_INIT;
    }
    else
    {
        ndrxj_atmi_throw(env, NULL, NULL, TPEINVAL, "buffer type [%s] not supported", 
                p_type);
        goto out;
    }
    
    NDRX_LOG(log_debug, "About to NewObject(%s)", clazz);
    
    ret = (*env)->NewObject(env, bclz, mid, ctx_obj, (jboolean)finalize, 
            (jlong)(long)data, len);
    
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
 * Extract C data buffer from TypedBuffer object
 * CONTEXT: Assume it is set.
 * @param env Java env
 * @param data ATMI buffer
 * @param buf return pointer to data block
 * @param len return value of buffer len (if applicable)
 * @param [out] doFinalize return current status for finalization (destructor).
 * @param [in] unsetDoFinalize remove finalize flag
 * @param [in] unsetPtr remove C pointers from buffer
 *  basically invalidate java buffer. As C side will take care of..
 * @return EXSUCCEED in case of OK, EXFAIL in case of failure
 * @thorws java exceptions if any
 */
expublic int ndrxj_atmi_TypedBuffer_get_buffer(JNIEnv *env, 
            jobject data, char **buf, long *len, jboolean *doFinalize, 
            int unsetDoFinalize, int unsetPtr)
{
    int ret = EXSUCCEED;
    jlong cptr;
    jlong clen;
    
    if (NULL==data)
    {
        /* just return NULL... */
        *buf = NULL;
        goto out;
    }
    
    cptr = (*env)->GetLongField(env, data, ndrxj_clazz_TypedBuffer_fid_cPtr);
    
    if (unsetPtr)
    {
        (*env)->SetLongField(env, data, ndrxj_clazz_TypedBuffer_fid_cPtr, 0L);
    }
    
    clen = (*env)->GetLongField(env, data, ndrxj_clazz_TypedBuffer_fid_len);
            
    
    *buf = (char *)(long)cptr;
    *len = (long)clen;
    
    if (NULL!=doFinalize || unsetDoFinalize)
    {

        if (NULL!=doFinalize)
        {
            *doFinalize = (*env)->GetBooleanField(env, data, 
                    ndrxj_clazz_TypedBuffer_fid_doFinalize);
        }
        
        if (unsetDoFinalize)
        {
            (*env)->SetBooleanField(env, data, 
                    ndrxj_clazz_TypedBuffer_fid_doFinalize, JNI_FALSE);
        }
    }
    
    /* check exception */
    if((*env)->ExceptionCheck(env))
    {
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_NDRX, 
                "Failed to transfer doFinalize flag between buffers: %s - %p",
                data);
        EXFAIL_OUT(ret);
    }
    
out:
    
    return ret;
}

/**
 * Update the buffer pointer to C data
 * @param env java env
 * @param data data buffer
 * @param buf new C pointer
 * @param len new data size
 * @return EXSUCCEED/EXFAIL
 */
expublic int ndrxj_atmi_TypedBuffer_set_buffer(JNIEnv *env, 
            jobject data, char *buf, long len)
{
    int ret = EXSUCCEED;
    jlong cptr = (jlong)(long)buf;
    jlong clen = (jlong)len;
    
    (*env)->SetLongField(env, data, ndrxj_clazz_TypedBuffer_fid_cPtr, cptr);
    (*env)->SetLongField(env, data, ndrxj_clazz_TypedBuffer_fid_len, clen);
    
out:
    
    return ret;
}

/**
 * Reallocate ATMI buffer to new size
 * @param env java env
 * @param data ATMI buffer object
 * @param size new buffer size
 */
expublic JNIEXPORT void JNICALL ndrxj_Java_org_endurox_TypedBuffer_tprealloc
  (JNIEnv *env, jobject data, jlong size)
{
    char *buf;
    long len;
    
    /* Switch context (get from buffer) */
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
        return;
    }
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &buf, &len, 
            NULL, EXFALSE, EXFALSE))
    {
        NDRX_LOG(log_error, "Failed to extract ATMI buffer from TypedBuffer Object");
        goto out;
    }
    
    /* Reallocate */
    if (NULL==(buf = tprealloc(buf, (long)size)))
    {
        NDRX_LOG(log_error, "Failed to reallocate buffer: %s", tpstrerror(tperrno));
        ndrxj_atmi_throw(env, NULL, NULL, tperrno, "%s", tpstrerror(tperrno));
        goto out;
    }
    
    /* Set buffer back/update object */
    
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_set_buffer(env, data, buf, len))
    {
        /* Exception must be set */
        NDRX_LOG(log_error, "Failed to update ATMI Buffer with reallocated data %p/%ld",
                buf, len);
        goto out;
    }
    
    NDRX_LOG(log_debug, "Atmi buffer object updated ok with %p/%ld", buf, len);
    
out:
    tpsetctxt(TPNULLCONTEXT, 0L);
}

/**
 * Prepare Typed buffer result i.e. check if pointer is changed, then
 * check types, if types are the same, then just update "data" object.
 * If types are changed, the "data" object shall be marked as destructor
 * not needed and allocate new object according to the "odata" buffer.
 * Note the exception is not checked here...
 * 
 * @param env java env
 * @param ctx_obj ATMI Context obj
 * @param data original data object
 * @param idata input data pointer
 * @param ilen input data len
 * @param odata output (result) data buffer
 * @param olen output len
 * @param itype input buffer ATMI type
 * @param isubtype input buffer ATMI sub-type
 * @return final java object according to the "odata". Might be re-used
 *  "data" object or new.
 */
expublic jobject ndrxj_atmi_TypedBuffer_result_prep
  (JNIEnv *env, jobject ctx_obj, jobject data, char *idata, 
        long ilen, char *odata, long olen, char *itype, char *isubtype)
{
    jobject ret = NULL;
    
    char otype[XATMI_TYPE_LEN+1]  = "NULL";
    char osubtype[XATMI_SUBTYPE_LEN+1]  = {EXEOS};
    
    int is_types_eq;
    
    jboolean finalizeOrg = EXFALSE;

    if (NULL!=odata)
    {
        if (EXFAIL==tptypes(odata, otype, osubtype))
        {
            NDRX_LOG(log_error, "Failed to get odata type infos: %s", 
                    tpstrerror(tperrno));
            ndrxj_atmi_throw(env, NULL, NULL, tperrno, "Failed to get odata type infos: %s", 
                    tpstrerror(tperrno));
            goto out;
        }
    }
    
    if (0==strcmp(otype, itype))
    {
        is_types_eq = EXTRUE;
    }
    else
    {
        is_types_eq = EXFALSE;
    }
    
    /* What do we want to do in case if buffer pointer is changed? */
    
    if (is_types_eq && idata==odata && ilen==olen)
    {
        NDRX_LOG(log_debug, "ptr, types and len not changed...");
        ret = data;
    }
    else if (is_types_eq)
    {    
        ret = data;
        
        if (ilen!=olen)
        {
            NDRX_LOG(log_debug, "Buffer sizes changed...");

            (*env)->SetLongField(env, data, ndrxj_clazz_TypedBuffer_fid_len, 
                    (jlong)olen);
        }
        
        if (idata!=odata && NULL!=data)
        {
            NDRX_LOG(log_debug, "Buffer pointers changed...");

            (*env)->SetLongField(env, data, ndrxj_clazz_TypedBuffer_fid_cPtr, 
                    (jlong)(long)odata);

        }
    }
    else
    {
        /* deactivate the original buffer and allocate new for odata (if odata is NULL
         * then return TypedNULL. But we could also return NULL of the buffer object
         * !!! Enduro/X must delete the incoming buffer if return is NULL
         */

        if (NULL!=data)
        {
            /* Change the not finalize flag */

            finalizeOrg = (*env)->GetBooleanField(env, data, 
                    ndrxj_clazz_TypedBuffer_fid_doFinalize);

            (*env)->SetBooleanField(env, data, ndrxj_clazz_TypedBuffer_fid_doFinalize, 
                    (jboolean)JNI_FALSE);
        }
        else
        {
            /* null buffers always are automatic */
            finalizeOrg = EXTRUE;
        }

        /* now allocate new typed buffer */

        if (NULL==odata)
        {
            NDRX_LOG(log_debug, "NULL buffer");
            ret = NULL;
        }
        else
        {
            /* exception shall be set */
            if (NULL==(ret = ndrxj_atmi_TypedBuffer_translate(env, 
                ctx_obj, EXTRUE, odata, olen,
                otype, osubtype, finalizeOrg)))
            {
                NDRX_LOG(log_error, "Failed to translate buffer %p", odata);
                goto out;
            }
        }
    }
    
out:
    
    return ret;
}

/**
 * Extract C side type information
 * @param env java env
 * @param data ATMI Buffer
 * @return TpTypesResult object
 */
expublic JNIEXPORT jobject JNICALL ndrxj_Java_org_endurox_TypedBuffer_tptypes
  (JNIEnv * env, jobject data)
{
    char btype[16] = {EXEOS};
    char stype[16] = {EXEOS};
    long size;
    char *cdata;
    long clen;
    jobject ret = NULL;
    
    if (NULL==ndrxj_TypedBuffer_get_ctx(env, data, EXTRUE))
    {
       return NULL; 
    }
    
    /* get atmi buffer */
    if (EXSUCCEED!=ndrxj_atmi_TypedBuffer_get_buffer(env, data, &cdata, &clen,
            NULL, EXFALSE, EXFALSE))
    {
        NDRX_LOG(log_error, "Failed to get buffer data");
        goto out;
    }
    
    /* read type infos */
    if (EXFAIL==(size = tptypes(cdata, btype, stype)))
    {
        ndrxj_atmi_throw(env, data, NULL, tperrno, "%s", tpstrerror(tperrno));
        goto out;
    }
    
    ret = ndrxj_TpTypesResult_new(env, btype, stype, size);
    
out:
    tpsetctxt(TPNULLCONTEXT, 0L);

    return ret;
    
}

/* vim: set ts=4 sw=4 et smartindent: */
