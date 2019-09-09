/**
 * @brief Classpath handler
 *
 * @file cp.c
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
#include <stdio.h>
#include <errno.h>
#include <jni.h>
#include <ndebug.h>
#include <userlog.h>
#include <ndrstandard.h>
#include <inicfg.h>
#include <cconfig.h>
#include <exjlds.h>
#include <nstdutil.h>
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
#define MAX_TAGLEN              10  /**<  this is for -classpath */
#define URL_CLASS               "java/net/URL"
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/

/**
 * Return array object of URLs
 * @param[in] env java env
 * @param[in] list array URLs/pathes
 * @return NULL or array object
 */
expublic jobjectArray ndrxj_build_classpath_urls(JNIEnv *env, string_list_t *list)
{
    int ret = EXSUCCEED;
    jobjectArray retArr = NULL;
    int count = 0;
    jclass clazz = NULL;
    string_list_t *elm;
    int i;
    jmethodID mid;
    char resource[PATH_MAX];
    jobject obj = NULL;
    jstring jstr = NULL;
    
    elm = list;
    if (NULL!=elm)
    {
        do
        {
            count++;
        } 
        while (NULL!=(elm=elm->next));
    }
    
    clazz = (*env)->FindClass(env, URL_CLASS);

    if (NULL==clazz)
    {        
        /* I guess we need to abort here! */
        NDRX_LOG(log_error, "Failed to to get [%s] class!", URL_CLASS);
        EXFAIL_OUT(ret);
    }

    /* create buffer object... */
    mid = (*env)->GetMethodID(env, clazz, "<init>", "(Ljava/lang/String;)V");
    
    if (NULL==mid)
    {
        NDRX_LOG(log_error, "Cannot get buffer constructor!");
        goto out;
    }

    
    retArr = (*env)->NewObjectArray( env, count, clazz, NULL);
    
    if (NULL==retArr)
    {
        NDRX_LOG(log_error, "Failed to allocate array of [%s] count %d",
                URL_CLASS, count);
        EXFAIL_OUT(ret);
    }

    elm = list;
    
    i = 0;
    if (NULL!=elm)
    {
        do
        {
            snprintf(resource, sizeof(resource), "file://%s", elm->qname);

            NDRX_LOG(log_debug, "Adding classpath resource [%s]", resource);

            jstr = (*env)->NewStringUTF(env, resource);

            if (NULL==jstr)
            {
                NDRX_LOG(log_error, "Failed to create java string for [%s]!",
                        jstr);
                EXFAIL_OUT(ret);
            }

            NDRX_LOG(log_debug, "About to new obj [%s]", URL_CLASS);

            obj = (*env)->NewObject(env, clazz, mid, jstr);

            if (NULL==obj)
            {
                NDRX_LOG(log_error, "Failed to create object of [%s]", URL_CLASS);
                EXFAIL_OUT(ret);
            }

            /* remove local ref */
            (*env)->DeleteLocalRef( env, jstr);
            jstr=NULL;

            (*env)->SetObjectArrayElement(env, retArr, i, obj);

            /* remove local object */
            (*env)->DeleteLocalRef( env, obj);
            obj = NULL;

            i++;

        }
        while (NULL!=(elm=elm->next));
    }
    
out:
   
    if (NULL!=clazz)
    {
        (*env)->DeleteLocalRef( env, clazz);
    }

    /* remove any left overs in case of failure: */
    if (NULL!=jstr)
    {
        (*env)->DeleteLocalRef( env, jstr);
    }

    if (NULL!=obj)
    {
        (*env)->DeleteLocalRef( env, obj);
    }

    if (EXSUCCEED!=ret)
    {
        if (NULL!=retArr)
        {
            (*env)->DeleteLocalRef( env, retArr);
            retArr = NULL;
        }
        retArr = NULL;
    }

    return retArr;
}

/**
 * Process classpath if any
 * @param config_line configuratoin string
 * @return EXSUCCEED/EXFAIL
 */
expublic int ndrxj_cp_proc(string_list_t**list, char *config_line)
{
    int ret = EXSUCCEED;
    char *p = config_line;
    char buf[100];
    char *tag = buf;
    int taglen = 0;
    
    while (' '==*p || '\t'==*p)
    {
        p++;
        
        if (EXEOS==*p)
        {
            break;
        }
    }
    
    /* find the tag ending */
    while (' '!=*p && '\t'!=*p && EXEOS!=*p)
    {
        *tag = *p;
        
        tag++;
        p++;
        taglen++;
        
        if (MAX_TAGLEN < taglen)
        {
            break;
        }
    }
    
    buf[taglen] = EXEOS;
    
    /* check that we got till the whitespace */
    if (' ' == config_line[taglen] ||
            '\t' == config_line[taglen])
    {
        if (0==strcmp(buf, "-cp") || 0==strcmp(buf, "-classpath"))
        {
            NDRX_LOG(log_debug, "Got classpath argument -> append to list");
            
            /* we need need a function to parse the string and load
             * it in common string list
             */
            if (EXSUCCEED!=ndrx_string_list_splitadd(list, config_line+taglen, ":"))
            {
                NDRX_LOG(log_error, "Failed to build classpath list from CLI");
                EXFAIL_OUT(ret);
            }
        }
    }
    
out:
    return ret;
}


/* vim: set ts=4 sw=4 et smartindent: */
