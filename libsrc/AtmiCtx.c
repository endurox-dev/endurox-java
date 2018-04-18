#include <jni.h>
#include "org_endurox_AtmiCtx.h"
#include <atmi.h>
#include <oatmi.h>
#include <ndebug.h>

/**
 * Class:     HelloJNI
 * Method:    sayHello
 * Signature: ()V
 */
jlong JNICALL Java_org_endurox_AtmiCtx__tpnewctxt (JNIEnv *env, jclass cls)
{
        char *ctx = tpnewctxt(0, 0);
        return (long)ctx;
}

/**
 * Get ATMI Error result
 */
JNIEXPORT jobject JNICALL Java_org_endurox_AtmiCtx_getAtmiError (JNIEnv *env, jobject obj)
{
    TPCONTEXT_T *p_ctxt;
    int err;
    
    jstring jstr;
    jclass objClass = (*env)->FindClass(env, obj);
    jfieldID myFieldID = (*env)->GetFieldID(env, objClass, "ctx", "J");
    jlong fieldVal = (*env)->GetLongField(env, obj, myFieldID);
    
    
    p_ctxt = (TPCONTEXT_T *)fieldVal;
    NDRX_LOG(log_debug, "context: %ld (%p)", fieldVal, p_ctxt);
    err = Otperrno(p_ctxt);
    

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

    jstr=(jstring)((*env)->NewStringUTF(env, Otpstrerror(p_ctxt, err)) );
    (*env)->SetObjectField(env, errObj,param2Field,(jobject)jstr);

    /* return object */
    return obj;
}

