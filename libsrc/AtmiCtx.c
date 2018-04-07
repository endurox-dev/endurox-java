#include <jni.h>
#include "AtmiCtx.h"
#include <atmi.h>

/**
 * Class:     HelloJNI
 * Method:    sayHello
 * Signature: ()V
 */
jlong JNICALL Java_AtmiCtx_tpnewctxt (JNIEnv *env, jclass cls)
{
        char *ctx = tpnewctxt(0, 0);
        return (long)ctx;
}
 
