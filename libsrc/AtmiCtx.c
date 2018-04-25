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
#include "org_endurox_AtmiCtx.h"
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
 * Throw runtime exception 
 */
jint ndrxj_throw_exception(JNIEnv *env, char *msg)
{
    return (*env)->ThrowNew(env, (*env)->FindClass(env, "java/lang/Exception"), msg);
}

/**
 * Class:     HelloJNI
 * Method:    sayHello
 * Signature: ()V
 */
jlong JNICALL Java_org_endurox_AtmiCtx_tpnewctxt (JNIEnv *env, jclass cls)
{
        TPCONTEXT_T ctx = tpnewctxt(0, 0);
        if (NULL==ctx)
        {
            ndrxj_atmi_throw(env, TPESYSTEM, "Failed to allocate new ATMI context!");
        }
        NDRX_LOG(log_debug, "New ATMI context: %p", ctx);
        return (long)ctx;
}

/**
 * Get ATMI Error result
 */
JNIEXPORT jobject JNICALL Java_org_endurox_AtmiCtx_getAtmiError (JNIEnv *env, jobject obj)
{
    TPCONTEXT_T ctx;
    int err;
    
    jstring jstr;
    jclass objClass = (*env)->GetObjectClass(env, obj);
    jfieldID myFieldID = (*env)->GetFieldID(env, objClass, "ctx", "J");
    jlong fieldVal = (*env)->GetLongField(env, obj, myFieldID);
    
    ctx = (TPCONTEXT_T)fieldVal;
    tpsetctxt(ctx, 0L);

    NDRX_LOG(log_debug, "context: %ld (%p)", fieldVal, ctx);
    err = tperrno;

    /* Get the class we wish to return an instance of */
    jclass errClazz = (*env)->FindClass(env, "org/endurox/ErrorTuple");

    /* Get the method id of an empty constructor in clazz */
    jmethodID constructor = (*env)->GetMethodID(env, errClazz, "<init>", "()V");

    /* Create an instance of clazz */
    jobject errObj = (*env)->NewObject(env, errClazz, constructor);

    /* Get Field references */
    jfieldID param1Field = (*env)->GetFieldID(env, errClazz, "err", "I");
    jfieldID param2Field = (*env)->GetFieldID(env, errClazz, "msg", "Ljava/lang/String;");

    /* Set fields for object */
    (*env)->SetIntField(env, errObj, param1Field, err);

    jstr=(jstring)((*env)->NewStringUTF(env, tpstrerror(err)) );
    (*env)->SetObjectField(env, errObj,param2Field,(jobject)jstr);
    
    /* throw some exception, for test!!! */
    ndrxj_atmi_throw(env, TPENOENT, "HELLO TEST EXCEPTION!!!!");
    
    NDRX_LOG(log_error, "YOPT ! AFTER!");

    /* unset context */
    tpsetctxt(NULL, 0L);

    /* return object */
    return errObj;
}


