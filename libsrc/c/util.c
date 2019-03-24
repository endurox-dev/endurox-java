/**
 * @brief API Utilities to avoid repetitive source writing
 *
 * @file util.c
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
#include "org_endurox_TypedBuffer.h"
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
 * Convert Java object to C
 * @param env java env
 * @param ctx_obj 
 * @param tab conversion table, terminated with null field
 * @param clz java class descr
 * @param clzstr class string
 * @param jobj java object
 * @param cobj c object
 * @return EXSUCCEED/EXFAIL (exception set)
 */
expublic int ndrxj_cvt_to_c(JNIEnv *env, 
            jobject ctx_obj, exjobjmap_t *tab, jclass clz, char *clzstr,
            jobject jobj, void *cobj)
{
    int ret = EXSUCCEED;
    jfieldID fid;
    
    while (tab->field)
    {
        
        if (NULL==(fid = (*env)->GetFieldID(env, clz, tab->field, tab->ftype)))
        {
            NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_NDRX, 
                    "Failed to get [%s] descr from %s: %s", tab->field, clzstr);
            EXFAIL_OUT(ret);
        }
       
        /* read the field accordingly */
        if (0==strcmp(tab->ftype, "J"))
        {
            long *cfld = (long *)(((char *)cobj)+ tab->coffset);
            *cfld = (long)(*env)->GetLongField(env, jobj, fid);
        }
        else if (0==strcmp(tab->ftype, "Ljava/lang/String;"))
        {
            jboolean n_str_copy = EXFALSE;
            const char *n_str = "";
            jstring jstr;
            char *cstr = ((char *)cobj)+ tab->coffset;
            jstr = (jstring)(*env)->GetObjectField(env, jobj, fid);
            
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
            
            NDRX_STRNCPY(cstr, n_str, tab->csz);
            
            if (n_str_copy)
            {
                (*env)->ReleaseStringUTFChars(env, jstr, n_str);
            }
            
        }
        else if (0==strcmp(tab->ftype, "[B"))
        {
            jboolean n_carray_copy = EXFALSE;
            jbyteArray jb = (jbyteArray)(*env)->GetObjectField(env, jobj, fid);
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
                    ndrxj_atmi_throw(env, NULL, TPEINVAL, "Class [%s] field [%s] "
                            "expected len in C %d but got from java %d", clzstr, 
                            tab->field, tab->csz, len);
                    
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
        
        if((*env)->ExceptionCheck(env))
        {
            NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_NDRX, 
                    "Failed to convert C to Java Struct: %s");
            EXFAIL_OUT(ret);
        }
        
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
 * @param clz java class descr
 * @param clzstr class string
 * @param cobj c object
 * @param jobj java object
* @return EXSUCCEED/EXFAIL (exception set)
 */
expublic int ndrxj_cvt_to_java(JNIEnv *env, 
            jobject ctx_obj, exjobjmap_t *tab, jclass clz, char *clzstr,
            void *cobj, jobject jobj)
{
    int ret = EXSUCCEED;
    jfieldID fid;
    
    while (tab->field)
    {
        if (NULL==(fid = (*env)->GetFieldID(env, clz, tab->field, tab->ftype)))
        {
            NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_NDRX, 
                    "Failed to get [%s] descr from %s: %s", tab->field, clzstr);
            EXFAIL_OUT(ret);
        }
       
        /* read the field accordingly */
        if (0==strcmp(tab->ftype, "J"))
        {
            long *cfld = (long *)(((char *)cobj)+ tab->coffset);
            (*env)->SetLongField(env, jobj, fid, (jlong)*cfld);
        }
        else if (0==strcmp(tab->ftype, "Ljava/lang/String;"))
        {
            char *cfld = ((char *)cobj)+ tab->coffset;
            jstring str = (*env)->NewStringUTF(env, cfld);
            (*env)->SetObjectField(env, jobj, fid, str);
        }
        else if (0==strcmp(tab->ftype, "[B"))
        {
            char *cfld = ((char *)cobj)+ tab->coffset;
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

        } /* if [B */
        
        
        if((*env)->ExceptionCheck(env))
        {
            NDRXJ_LOG_EXCEPTION(env, log_error, NDRXJ_LOGEX_NDRX, 
                    "Failed to convert C to Java Struct: %s");
            EXFAIL_OUT(ret);
        }
        
        tab++;
    }
    
out:
    
    NDRX_LOG(log_debug, "returns %d", ret);
    
    return ret;
    
}

/* vim: set ts=4 sw=4 et smartindent: */
