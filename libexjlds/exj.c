/**
 * @brief Enduro/X Linker static library (goes into target binary as loader)
 *
 * @file exj.h
 */ 
/*
 * -----------------------------------------------------------------------------
 * Enduro/X Middleware Platform for Distributed Transaction Processing
 * Copyright (C) 2015-2018 Mavimax, Ltd. All Rights Reserved.
 * This software is released under one of the following licenses:
 * GPL or Mavimax's license for commercial use.
 * -----------------------------------------------------------------------------
 * GPL license:
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA 02111-1307 USA
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

/* Include generated resources */
#include "StaticClassLoader.cinclude"

/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
exprivate jclass M_classLoaderClass = NULL;
exprivate jobject M_classLoader = NULL;
exprivate jmethodID M_class_getctors_method;

/*---------------------------Prototypes---------------------------------*/

/**
 * Log exception
 * @param ENV__ Java Env for which exception have occurred
 * @param LEV__ log level
 * @param ULOG__ use log EXTRUE/EXFALSE
 * @param FMT__ format string. Note first argument will be %s for stack trace
 * @param ... var arguments to format
 */
#define EXJLD_LOG_EXCEPTION(ENV__, LEV__, FMT__, ...) {\
\
    char *jerr__ = ndrxj_exjld_exception_backtrace(ENV__);\
    userlog(FMT__, jerr__, ##__VA_ARGS__);\
    NDRX_LOG(LEV__, FMT__, jerr__, ##__VA_ARGS__);\
    NDRX_FREE(jerr__);\
}

#if 0
/**
 * Sorted class data
 */
exprivate ndrxj_class_index_t int_M_index [] = 
{
	
	{
		"oata.HelloWorld",
		ndrx_G_resource_HelloWorld,
		241
	}
	,
	{
		"oata.HelloY",
		ndrx_G_resource_HelloY,
		317
	}
	
};
#endif

/* library globals... JNI does not allow mulit threading, thus
 * lets leave them for global..
 */
exprivate ndrxj_class_index_t *M_index = NULL;
exprivate int M_index_len = 0;

/**
 * Return stacktrace of the exception into allocated buffer
 * @param env Java env where exception is set
 * @return allocated string with exception data
 */
expublic char *ndrxj_exjld_exception_backtrace(JNIEnv *env)
{
    jthrowable exc;
    jstring s;
    const char* utf;
    jboolean isCopy = EXFALSE;
    char *ret = NULL;

    jmethodID toString = (*env)->GetMethodID(env, 
            (*env)->FindClass(env, "java/lang/Object"), 
            "toString", "()Ljava/lang/String;");

    exc = (*env)->ExceptionOccurred(env);

    s = (jstring)(*env)->CallObjectMethod(env, exc, toString);

    utf = (*env)->GetStringUTFChars(env, s, &isCopy);

    ret = NDRX_STRDUP(utf);

    if (isCopy)
    {
            (*env)->ReleaseStringUTFChars(env, s, utf);
    }

    return ret;
}

/**
 * Get the bytes of the class
 * This is callback from java Static Class Loader.
 * Searches class into sorted array.
 * @param env Java ENV
 * @param ldrobj Java class loader instance
 * @param clsclass name to be loaded
 * @return instance to byte array of class data if found or NULL
 */
expublic jbyteArray getResourceBytes(JNIEnv * env, jobject ldrobj, jstring cls)
{
    jbyteArray ret = NULL;
    const char* utf;
    jboolean isCopy = EXFALSE;

    int c, first, last, middle;

    utf = (*env)->GetStringUTFChars(env, cls, &isCopy);

    NDRX_LOG(log_debug, "Loading class [%s]", utf);

    /* perform binary search for class data */

    first = 0;
    last = M_index_len - 1;
    middle = (first+last)/2;

    while (first <= last) 
    {
        /* -1 cls < utf */
        /* 0 cls == utf */
        /* 1 cls > utf */
        int res = strcmp(M_index[middle].cls, utf);

        if (res < 0)
        {
            first = middle + 1;    
        }
        else if (res==0) 
        {
            NDRX_LOG(log_debug, "%s found at location %d - loading...", 
                     utf, middle);
            break;
        }
        else
        {
            last = middle - 1;
        }

        middle = (first + last)/2;
    }

    if (first > last)
    {
        NDRX_LOG(log_debug, "%s not found - ignore", utf);
        goto out;
    }


    ret = (*env)->NewByteArray(env, M_index[middle].len);

    if (NULL==ret)
    {
        EXJLD_LOG_EXCEPTION(env, log_error, 
                "Failed to allocate Byte Array: %s (%d)", M_index[middle].len);
        goto out;
    }

    (*env)->SetByteArrayRegion(env, ret, 0, M_index[middle].len, 
                            (jbyte*)M_index[middle].data);

    if((*env)->ExceptionCheck(env))
    {
        EXJLD_LOG_EXCEPTION(env, log_error, 
                        "Failed to set class data bytes: %s (%d)",
                        M_index[middle].len
                        );
        ret = NULL;
        goto out;
    }
	
out:

    if (isCopy)
    {
        (*env)->ReleaseStringUTFChars(env, cls, utf);
    }
	
	return ret;
}

/**
 * Create/load the Enduro/X Static Class Loader
 * @param env java env
 * @param vm Java virtual machine
 * @return EXSUCCEED or EXFAIL
 */
exprivate int create_loader(JNIEnv *env, JavaVM *vm)
{
    int ret = EXSUCCEED;
    jclass loaderClass;
    jmethodID loaderMethod;
    jobject loader;
    jclass class_class;
    jmethodID ldr_ctor;
    jobject ldr_obj;
    JNINativeMethod m[1];
    jclass cl;

    loaderClass = (*env)->FindClass(env, "java/lang/ClassLoader"); 
    if(NULL==loaderClass) {
        EXJLD_LOG_EXCEPTION(env, log_error, "Failed to load initial class loader: %s");
        EXFAIL_OUT(ret);
    }

    loaderMethod = (*env)->GetStaticMethodID(env, loaderClass, 
                    "getSystemClassLoader", "()Ljava/lang/ClassLoader;");

    if(NULL==loaderMethod) {

        EXJLD_LOG_EXCEPTION(env, log_error, 
                            "Failed to get getSystemClassLoader: %s");
        EXFAIL_OUT(ret);
    }

    loader = (*env)->CallStaticObjectMethod(env, loaderClass, loaderMethod);

    /* check the results  */
    if (NULL==loader)
    {
        EXJLD_LOG_EXCEPTION(env, log_error, 
                            "Failed to CallStaticObjectMethod of "
                            "system class loader: %s");
        EXFAIL_OUT(ret);
    }

    loader = (*env)->NewGlobalRef(env, loader);

    if (NULL==loader)
    {
            NDRX_LOG(log_error, "Failed to create global ref of loader");
            EXFAIL_OUT(ret);
    }

    cl = (*env)->DefineClass(env, "org/endurox/loader/StaticClassLoader", loader, 
                                (const jbyte*) ndrx_G_resource_StaticClassLoader, 
                                ndrx_G_resource_StaticClassLoader_len_def);

    if(!cl)
    {
        EXJLD_LOG_EXCEPTION(env, log_error, 
                            "Failed to load StaticClassLoader: %s");
        EXFAIL_OUT(ret);
    }

    M_classLoaderClass = (jclass) (*env)->NewGlobalRef(env, cl);

    /* Now link in native methods */
    m[0].fnPtr = getResourceBytes;
    m[0].name = "getResourceBytes";
    m[0].signature = "(Ljava/lang/String;)[B";

    (*env)->RegisterNatives(env, M_classLoaderClass, m, 1);

    if((*env)->ExceptionCheck(env))
    {
            EXJLD_LOG_EXCEPTION(env, log_error, 
                                "Failed to register native methods: %s");
            EXFAIL_OUT(ret);
    }

    ldr_ctor = (*env)->GetMethodID(env, M_classLoaderClass, "<init>", "()V");

    if(!ldr_ctor) 
{
        EXJLD_LOG_EXCEPTION(env, log_error, 
                            "Failed to get static loader constructor: %s");
        EXFAIL_OUT(ret);
    }

    ldr_obj = (*env)->NewObject(env, M_classLoaderClass, ldr_ctor);

    if(!ldr_obj) 
    {
        EXJLD_LOG_EXCEPTION(env, log_error, 
                            "Cannot create loader instance: %s");
        EXFAIL_OUT(ret);
    }

    M_classLoader = (*env)->NewGlobalRef(env, ldr_obj);

    NDRX_LOG(log_info, "Static loader ready for boot...");
	
out:

    NDRX_LOG(log_debug, "%s returns %d", __func__, ret);

    return ret;
}

/**
 * Run the loader 
 * @param env Java env
 * @param main_class_str main class to be run
 * @param argc command line argument count
 * @param argv command line argument values
 * @param test_mode EXTRUE - perform class loader test, EXFALSE - do real run
 */
exprivate int run_ldr_main(JNIEnv *env, 
			char *main_class_str, 
			int argc, char **argv,
			int test_mode
  			)
{
    int ret = EXSUCCEED;
    jclass main_class;
    jclass str_class;
    jmethodID mid;

    jstring jstr; 
    jobjectArray args; 
    int i;
    jmethodID load_class;

    /* boot the main method of the class */

    load_class  = (*env)->GetMethodID(env, M_classLoaderClass, 
                            "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");
    if(NULL==load_class)
    {
        EXJLD_LOG_EXCEPTION(env, log_error, 
                            "Cannot get method 'findClass' of StreamLoader: %s");
        EXFAIL_OUT(ret);
    }

    /* get the main class */
    main_class = (jclass) (*env)->CallObjectMethod(env, M_classLoader, 
                                                   load_class, 
                                            (*env)->NewStringUTF(env, main_class_str));
    if (NULL==main_class)
    {
        EXJLD_LOG_EXCEPTION(env, log_error, 
                            "Failed to get main class: %s");
        EXFAIL_OUT(ret);
    }

    /* get main method... */

    mid = (*env)->GetStaticMethodID(env, main_class, "main", 
                                    "([Ljava/lang/String;)V"); 

    if (mid == 0)
    { 
        EXJLD_LOG_EXCEPTION(env, log_error, 
                            "Failed to get main method: %s");
        EXFAIL_OUT(ret);
    }

    /* fire it up! */

    /*
     * Build CLI..
     */
    str_class = (*env)->FindClass(env, "java/lang/String");

    if (NULL==str_class)
    {
        EXJLD_LOG_EXCEPTION(env, log_error, 
                "Cannot find String Class: %s");
        EXFAIL_OUT(ret);
    }

    args = (*env)->NewObjectArray(env, argc, str_class, jstr); 

    if (NULL==args)
    {
        EXJLD_LOG_EXCEPTION(env, log_error, 
                "Failed to allocate command line arguments: %s");
        EXFAIL_OUT(ret);
    }

    for (i=0; i<argc; i++)
    {
        jstring argString = (*env)->NewStringUTF(env, argv[i]);

        if (NULL==argString)
        {	
            EXJLD_LOG_EXCEPTION(env, log_error, 
                    "Failed to allocate cli arguments: %s (i=%d, argc=%d)",
                    i, argc);
            EXFAIL_OUT(ret);
        }

        /* set array elm */
        (*env)->SetObjectArrayElement(env, args, i, (jobject)argString);

        /* Check exception.. */
        if((*env)->ExceptionCheck(env))
        {
            EXJLD_LOG_EXCEPTION(env, log_error, 
                            "Failed to set args array elment: %s "
                            "(i=%d, argc=%d)",
                            i, argc);
            EXFAIL_OUT(ret);
        }

    }
	
    if (!test_mode)
    {
        NDRX_LOG(log_debug, "Starting...");
        /* boot up... */
        (*env)->CallStaticVoidMethod(env, main_class, mid, args);


        if((*env)->ExceptionCheck(env))
        {
            EXJLD_LOG_EXCEPTION(env, log_error, 
                            "Failed to run main: %s (%s)",
                            main_class_str);
            EXFAIL_OUT(ret);
        }

    }
	
out:
    NDRX_LOG(log_debug, "%s returns  %d", __func__, ret);

    return ret;
}

/**
 * Run java main
 * @param argc command line argument count
 * @param argv command line arguments
 * @param main_class main class entry
 * @param class_index index class data
 * @param class_index_len number of entries in index
 * @param emb_index embedded data index
 * @param emb_index_len number of entries in embedded index
 * @param test_mode do we run in test mode?
 * @return EXSUCCEED/EXFAIL
 */
expublic int ndrxj_run_main(int argc, char **argv, char *main_class,
                ndrxj_class_index_t *class_index, 
	        int class_index_len, ndrxj_class_index_t *emb_index, int emb_index_len,
                int test_mode)
{
    int ret = EXSUCCEED;
    JavaVM *vm = NULL;
    JNIEnv *env = NULL; 
    JavaVMInitArgs vm_args;
    jint res;

    ndrx_inicfg_t *cfg = NULL;
    ndrx_inicfg_section_keyval_t *out = NULL;
    ndrx_inicfg_section_keyval_t *val, *val_tmp;

    /* should be allocated... */
    JavaVMOption *options = NULL;

    int n_opt = 0;

    M_index = class_index;
    M_index_len = class_index_len;

    NDRX_LOG(log_debug, "Loading config...");
    /* Load Enduro/X based config... with @java section */
    if (EXSUCCEED!=ndrx_cconfig_load_general(&cfg))
    {
        NDRX_LOG(log_error, "Failed to get Enduro/X config handler: %s", 
                 Nstrerror(Nerror));
        EXFAIL_OUT(ret);  
    }

    /* Allocate multiple read setting which starts with "opts", and 
     * allocate java option for it
     */
    if (NULL!=cfg)
    {
        int cnt;

        if (EXSUCCEED!=ndrx_cconfig_get_cf(cfg, "@java", &out))
        {
            NDRX_LOG(log_error, "Failed to resolve [@debug] section: %s",
                    Nstrerror(Nerror));
            EXFAIL_OUT(ret);
        }

        /* might get bit extra keys, but not a problem... */
        cnt = EXHASH_COUNT(out);

        if (NULL==(options = NDRX_CALLOC(1, sizeof(JavaVMOption)*cnt)))
        {
            NDRX_LOG(log_error, "Failed to allocate: %d bytes: %s",
                    sizeof(JavaVMOption)*cnt, strerror(errno));
            EXFAIL_OUT(ret);
        }

        /* get the "opts" setting 
         * loop over the arguments... realloc and & set
         */

        EXHASH_ITER(hh, out, val, val_tmp)
        {
            NDRX_LOG(log_debug, "got: section [%s] key [%s] val [%s]", 
                     val->section, val->key, val->val);

            if (0==strncmp(val->key, "opts", 4))
            {

                options[n_opt].optionString = val->val;

                NDRX_LOG(log_debug, "Java option: [%s]", 
                         options[n_opt].optionString);

                n_opt++;
            }
        }
    }

    vm_args.version = JNI_VERSION_1_2;
    vm_args.options = options;
    vm_args.nOptions = n_opt;
    vm_args.ignoreUnrecognized = 1;

    NDRX_LOG(log_debug, "Creating JVM...");
    res = JNI_CreateJavaVM(&vm, (void **)&env, &vm_args);

    if (res < 0) 
    { 
        NDRX_LOG(log_error, "Failed to create Java VM");
        EXFAIL_OUT(ret);
    }

    /* prepare stream loader */

    NDRX_LOG(log_debug, "Preparing class loader...");
    if (EXSUCCEED!=create_loader(env, vm))
    {
        NDRX_LOG(log_error, "Failed to prepare class loader");
        EXFAIL_OUT(ret);
    }

    NDRX_LOG(log_debug, "Running main...");
    /* for java args does not contain binary name... */
    if (EXSUCCEED!=run_ldr_main(env, main_class, argc-1, &(argv[1]), test_mode))
    {
        NDRX_LOG(log_error, "Failed to run main");
        EXFAIL_OUT(ret);
    }

out:

    if (NULL!=options)
    {
        NDRX_FREE(options);
    }

    if (NULL!=out)
    {
        ndrx_keyval_hash_free(out);
    }

    if (NULL!=cfg)
    {
        ndrx_inicfg_free(cfg); 
    }

    return ret;
} 
