/**
 * @brief API Utilities to avoid repetitive source writing
 *   WARNING! We are during C initiated JVM calls here. Thus there is no class
 *   loader context for C. We use own method ndrxj_FindClass() here.
 *
 * @file util.c
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
#include <tmenv.h>
#include "nerror.h"
#include <ndrstandard.h>
#include <exjldsys.h>
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
#define EXXID_CLASS     "org/endurox/ExXid"
#define XID_CLASS       "javax/transaction/xa/Xid"
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/

/**
 * Convert Java object to C
 * @param env java env
 * @param ctx_obj 
 * @param tab conversion table, terminated with null field
 * @param clzstr class string
 * @param jobj java object
 * @param cobj c object
 * @return EXSUCCEED/EXFAIL (exception set)
 */
expublic int ndrxj_cvt_to_c(JNIEnv *env, 
            jobject ctx_obj, exjobjmap_t *tab, char *clzstr,
            jobject jobj, void *cobj)
{
    int ret = EXSUCCEED;
    
    while (tab->field)
    {
        /* read the field accordingly */
        if (0==strcmp(tab->ftype, "J"))
        {
            long *cfld = (long *)(((char *)cobj)+ tab->coffset);
            *cfld = (long)(*env)->GetLongField(env, jobj, *tab->fid);
        }
        else if (0==strcmp(tab->ftype, "Ljava/lang/String;"))
        {
            jboolean n_str_copy = EXFALSE;
            const char *n_str = "";
            jstring jstr;
            char *cstr = ((char *)cobj)+ tab->coffset;
            jstr = (jstring)(*env)->GetObjectField(env, jobj, *tab->fid);
            
            if (NULL!=jstr)
            {
                n_str = (*env)->GetStringUTFChars(env, jstr, &n_str_copy);
                
                /* we got exception... */
                if (NULL==n_str)
                {
                    NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_NDRX, 
                        "Failed to get [%s] descr from %s: %s", tab->field, clzstr);
                    EXFAIL_OUT(ret);
                }
                
            }
            
            NDRX_STRNCPY_SAFE(cstr, n_str, tab->csz);
            
            if (n_str_copy)
            {
                (*env)->ReleaseStringUTFChars(env, jstr, n_str);
            }
            
        }
        else if (0==strcmp(tab->ftype, "[B"))
        {
            jboolean n_carray_copy = EXFALSE;
            jbyteArray jb = (jbyteArray)(*env)->GetObjectField(env, jobj, *tab->fid);
            char * n_carray = NULL;
            jsize len;
            char *cmem = ((char *)cobj)+ tab->coffset;
            
            if (NULL!=jb)
            {
                n_carray = (char*)(*env)->GetByteArrayElements(env, jb, &n_carray_copy);
                
                if (NULL==n_carray)
                {
                    NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_NDRX, 
                        "Failed to get [%s] descr from %s: %s", tab->field, clzstr);
                    EXFAIL_OUT(ret);
                }
                
                len = (*env)->GetArrayLength(env, jb);
                
                /* The length must equal */
                
                if (len!=tab->csz)
                {
                    NDRX_LOG(log_error, "Class [%s] field [%s] "
                            "expected len in C %d but got from java %d", clzstr, 
                            tab->field, tab->csz, len);
                    ndrxj_atmi_throw(env, NULL, NULL, TPEINVAL, "Class [%s] field [%s] "
                            "expected len in C %d but got from java %d", clzstr);
                    
                    if(n_carray_copy)
                    {
                       (*env)->ReleaseByteArrayElements(env, jb, n_carray, JNI_ABORT);
                    }


                    EXFAIL_OUT(ret);
                }
                
                memcpy(cmem, n_carray, len);
                
                if(n_carray_copy)
                {
                   (*env)->ReleaseByteArrayElements(env, jb, n_carray, JNI_ABORT);
                }
            } /* if ! NULL */
            else
            {
                /* well set to 0? */
                memset(cmem, 0, tab->csz);
            }
        } /* if [B */
        
        tab++;
    }
    
out:
    
    NDRX_LOG(log_debug, "returns %d", ret);
    
    return ret;
    
}


/**
 * Convert C object to Java.
 * Java object must be pre-allocated.
 * @param env java env
 * @param ctx_obj 
 * @param tab conversion table, terminated with null field
 * @param clzstr class string
 * @param cobj c object
 * @param jobj java object
* @return EXSUCCEED/EXFAIL (exception set)
 */
expublic int ndrxj_cvt_to_java(JNIEnv *env, 
            jobject ctx_obj, exjobjmap_t *tab, char *clzstr,
            void *cobj, jobject jobj)
{
    int ret = EXSUCCEED;
    
    while (tab->field)
    {
        /* read the field accordingly */
        if (0==strcmp(tab->ftype, "J"))
        {
            long *cfld = (long *)(((char *)cobj)+ tab->coffset);
            (*env)->SetLongField(env, jobj, *tab->fid, (jlong)*cfld);
        }
        else if (0==strcmp(tab->ftype, "Ljava/lang/String;"))
        {
            char *cfld = ((char *)cobj)+ tab->coffset;
            jstring str = (*env)->NewStringUTF(env, cfld);
            (*env)->SetObjectField(env, jobj, *tab->fid, str);
            (*env)->DeleteLocalRef( env, str);
        }
        else if (0==strcmp(tab->ftype, "[B"))
        {
            char *cfld = ((char *)cobj)+tab->coffset;
            jbyteArray jb = NULL;
    
            jb = (*env)->NewByteArray(env, (jsize)tab->csz);

            if (NULL==jb)
            {
                NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_ULOG, 
                        "Failed to create byte array with: %s, size: %d", 
                        (int)tab->csz);
                EXFAIL_OUT(ret);
            }
            (*env)->SetByteArrayRegion(env, jb, 0, tab->csz, (jbyte*)cfld);
            (*env)->SetObjectField(env, jobj, *tab->fid, jb);
            
            /* Remove local reference to byte array... */
            (*env)->DeleteLocalRef( env, jb);

        } /* if [B */
        
        tab++;
    }
    
out:
    
    NDRX_LOG(log_debug, "returns %d", ret);
    
    return ret;
    
}

/**
 * Convert java string to c
 * @param env java env
 * @param ctx_obj ATMI context
 * @param jstr java string
 * @param outstr C string
 * @param outstrsz C string size
 * @return EXSUCCEED/EXFAIL
 */
expublic int ndrxj_cvt_jstr_to_c(JNIEnv *env, 
            jobject ctx_obj, jstring jstr, char *outstr, int outstrsz)
{
    int ret = EXSUCCEED;
    
    jboolean n_str_copy = EXFALSE;
    const char *n_str = "";
    
    n_str = (*env)->GetStringUTFChars(env, jstr, &n_str_copy);
                
    /* we got exception... */
    if (NULL==n_str)
    {
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_NDRX, 
            "Failed to convert to string: %s");
        EXFAIL_OUT(ret);
    }

    NDRX_STRNCPY_SAFE(outstr, n_str, outstrsz);

    if (n_str_copy)
    {
        (*env)->ReleaseStringUTFChars(env, jstr, n_str);
    }
    
out:
    return ret;
}

/**
 * Convert array from C string list to java
 * @param env java env
 * @param list Enduro/X string list
 * @param nrel number of elements in string list
 * @return Java array of string
 * @return NULL on failure
 */
expublic jobjectArray ndrxj_cvt_arr_c_to_java(JNIEnv *env, string_list_t *list, int nrel)
{
    jstring str;
    jobjectArray ret = NULL;
    int i;
    string_list_t *el;
    
    ret = (*env)->NewObjectArray(env, (jsize)nrel, ndrxj_clazz_String, 0);

    for (el=list, i=0; el; el=el->next, i++)
    {
        str = (*env)->NewStringUTF(env, el->qname);

        (*env)->SetObjectArrayElement(env,ret,i,str);
        (*env)->DeleteLocalRef(env, str);
    }
    
    return ret;
}

/**
 * Convert XID from C to Enduro/X ExXid
 * @param env java env
 * @param xid C Xid
 * @return NULL or ExXid
 */
expublic jobject ndrxj_cvt_xid_to_java(JNIEnv *env, XID *xid)
{
    jobject ret = NULL;
    
    /* OK we to convert data array to java array */
    
    jbyteArray jb = NULL;
    
    jb = (*env)->NewByteArray(env, (jsize)sizeof(xid->data));

    if (NULL==jb)
    {
        NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_ULOG, 
                "Failed to create byte array of size %d: %s", 
                sizeof(xid->data));
        ret = NULL;
        goto out;
    }

    (*env)->SetByteArrayRegion(env, jb, 0, sizeof(xid->data), (jbyte*)xid->data);
    
    /* Create that ExXid object */
    
    NDRX_LOG(log_debug, "About to NewObject(%s)", EXXID_CLASS);
    
    ret = (*env)->NewObject(env, ndrxj_clazz_ExXid, ndrxj_clazz_ExXid_mid_INIT, 
            xid->formatID, xid->gtrid_length, xid->bqual_length, jb);
    
    /* check result.. */
    
    if (NULL==ret)
    {
        ndrx_ctx_priv_t *ctxpriv;    
        ctxpriv = ndrx_ctx_priv_get();
            
        NDRX_LOG(log_error, "Failed to create java ExXid!");
        
        /* TODO: Backtrace the call? 
         * have some common logging for XA.
         */
        NDRXJ_LOG_EXCEPTION(NDRXJ_JENV(ctxpriv), log_error, 
            NDRXJ_LOGEX_NDRX, "Failed to create ExXid:%s");
        goto out;
    }

out:
            
    if (NULL!=jb)
    {
        (*env)->DeleteLocalRef( env, jb);
    }
    
    return ret;
}

/**
 * Convert Java XID to C
 * @param[in] env java env
 * @param[in] j_xid java xid
 * @param[out] c_xid converted C xid
 * @return EXSUCCEED/EXFAIL
 */
expublic int ndrxj_cvt_xid_to_c(JNIEnv *env, jobject j_xid, XID *c_xid)
{
    int ret = EXSUCCEED;
    jbyteArray jbqa = NULL;
    jbyteArray jgtid = NULL;
    int jbqa_len, jgtid_len;
    long formatId;
    
    jboolean n_bqa_copy = EXFALSE;
    char * n_bqa = NULL;
    
    jboolean n_gtid_copy = EXFALSE;
    char * n_gtid = NULL;
    
    /* get format id */
    formatId = (*env)->CallLongMethod(env, j_xid, ndrxj_clazz_Xid_mid_getFormatId);
    
    NDRX_LOG(log_debug, "Got format id: %ld", formatId);
    
    jbqa = (*env)->CallObjectMethod(env, j_xid, ndrxj_clazz_Xid_mid_getBranchQualifier);
    jbqa_len = (*env)->GetArrayLength(env, jbqa);
    
    NDRX_LOG(log_debug, "Branch qualifier len: %d", jbqa_len);
    
    /* Get tranid bytes */
    
    jgtid = (*env)->CallObjectMethod(env, j_xid, ndrxj_clazz_Xid_mid_getGlobalTransactionId);
    jgtid_len = (*env)->GetArrayLength(env, jgtid);
    
    NDRX_LOG(log_debug, "Transaction id len len: %d", jgtid_len);
    
    /* build the xid */
    memset(c_xid, 0, sizeof(*c_xid));
    
    c_xid->formatID = formatId;
    c_xid->bqual_length = jbqa_len;
    c_xid->gtrid_length = jgtid_len;
    
    /* Copy off data portions */
    
    n_gtid = (char*)(*env)->GetByteArrayElements(env, jgtid, &n_gtid_copy);
    
    memcpy(c_xid->data, n_gtid, c_xid->gtrid_length);
    
    n_bqa = (char*)(*env)->GetByteArrayElements(env, jbqa, &n_bqa_copy);
    
    memcpy(c_xid->data+c_xid->gtrid_length, n_bqa, c_xid->bqual_length);
    
    NDRX_DUMP(log_debug, "Restored XID", c_xid, sizeof(c_xid));
    
out:

    if (NULL!=jbqa)
    {
        (*env)->DeleteLocalRef( env, jbqa);
    }

    if (NULL!=jgtid)
    {
        (*env)->DeleteLocalRef( env, jgtid);
    }

    if(n_gtid_copy)
    {
       (*env)->ReleaseByteArrayElements(env, jgtid, n_gtid, JNI_ABORT);
    }

    if(n_bqa_copy)
    {
       (*env)->ReleaseByteArrayElements(env, jbqa, n_bqa, JNI_ABORT);
    }

    return ret;
}

/* vim: set ts=4 sw=4 et smartindent: */
