/**
 * @brief Enduro/X Linker static library (goes into target binary as loader)
 *
 * @file exj.c
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
#include <stdio.h>
#include <errno.h>
#include <jni.h>
#include <ndebug.h>
#include <userlog.h>
#include <ndrstandard.h>
#include <inicfg.h>
#include <cconfig.h>
#include <exjldsys.h>
#include <nstdutil.h>
#include <exj.h>
/* Include generated resources */
#include "StaticClassLoader.cinclude"
#include "BytesURLHandler.cinclude"
#include "BytesURLConnection.cinclude"

/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
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
    char *jerr__ = ndrxj_ldr_exception_backtrace(ENV__, NULL);\
    (*ENV__)->ExceptionClear(ENV__);\
    userlog(FMT__, jerr__, ##__VA_ARGS__);\
    NDRX_LOG(LEV__, FMT__, jerr__, ##__VA_ARGS__);\
    NDRX_FREE(jerr__);\
}

/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
exprivate jclass M_classLoaderClass = NULL;
exprivate jobject M_classLoader = NULL;
exprivate jmethodID M_class_getctors_method;

/**
 * URL Based classpath to fallback to when we do not find the compile
 * bytes
 */
exprivate string_list_t *M_classpath_url = NULL;

/* library globals... JNI does not allow mulit threading, thus
 * lets leave them for global..
 */
exprivate ndrxj_class_index_t *M_class_index = NULL;
exprivate int M_class_index_len = 0;


exprivate ndrxj_class_index_t *M_res_index = NULL;
exprivate int M_res_index_len = 0;

/*---------------------------Prototypes---------------------------------*/

/**
 * Resolve stored 
 * @param env java env
 * @param ldrobj class loader object
 * @param cls class/resource to load
 * @param index index table to use for search
 * @param index_len length of resource index table
 * @return class bytes or NULL if not found
 */
exprivate jbyteArray getIndexBytes(JNIEnv * env, jobject ldrobj, jstring cls, 
        ndrxj_class_index_t *index, int index_len)
{
    jbyteArray ret = NULL;
    const char* utf;
    jboolean isCopy = EXFALSE;

    int first, last, middle;

    utf = (*env)->GetStringUTFChars(env, cls, &isCopy);

    NDRX_LOG(log_debug, "Loading class/resource [%s]", utf);

    /* perform binary search for class data */

    first = 0;
    last = index_len - 1;
    middle = (first+last)/2;

    while (first <= last) 
    {
        /* -1 cls < utf */
        /* 0 cls == utf */
        /* 1 cls > utf */
        int res = strcmp(index[middle].cls, utf);

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
        NDRX_LOG(log_debug, "%s not found - fallback to URL", utf);
        goto out;
    }


    ret = (*env)->NewByteArray(env, index[middle].len);

    if (NULL==ret)
    {
        EXJLD_LOG_EXCEPTION(env, log_error, 
                "Failed to allocate Byte Array: %s (%d)", index[middle].len);
        goto out;
    }

    (*env)->SetByteArrayRegion(env, ret, 0, index[middle].len, 
                            (jbyte*)index[middle].data);

    if((*env)->ExceptionCheck(env))
    {
        EXJLD_LOG_EXCEPTION(env, log_error, 
                        "Failed to set class data bytes: %s (%d)",
                        index[middle].len
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
 * Get the bytes of resource
 * This is callback from java Static Class Loader.
 * Searches class into sorted array.
 * @param env Java ENV
 * @param ldrobj Java class loader instance
 * @param clsclass name to be loaded
 * @return instance to byte array of class data if found or NULL
 */
expublic jbyteArray getResourceBytes(JNIEnv * env, jobject ldrobj, jstring cls)
{
    return getIndexBytes(env, ldrobj, cls, M_res_index, M_res_index_len);
}

/**
 * Get the bytes of class
 * This is callback from java Static Class Loader.
 * Searches class into sorted array.
 * @param env Java ENV
 * @param ldrobj Java class loader instance
 * @param clsclass name to be loaded
 * @return instance to byte array of class data if found or NULL
 */
expublic jbyteArray getClassBytes(JNIEnv * env, jobject ldrobj, jstring cls)
{
    return getIndexBytes(env, ldrobj, cls, M_class_index, M_class_index_len);
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
    jmethodID ldr_ctor;
    jobject ldr_obj;
    JNINativeMethod m[2];
    jclass cl, urlcon, urlh;
    jobjectArray cpUrls = NULL;
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
    
    /* define classes for Bytes URL Connection */
   
    urlcon = (*env)->DefineClass(env, "org/endurox/loader/BytesURLConnection", loader, 
                                (const jbyte*) ndrx_G_resource_BytesURLConnection, 
                                ndrx_G_resource_BytesURLConnection_len_def);
    
    if(!urlcon)
    {
        EXJLD_LOG_EXCEPTION(env, log_error, 
                            "Failed to define BytesURLConnection: %s");
        EXFAIL_OUT(ret);
    }
    
    urlcon = (jclass) (*env)->NewGlobalRef(env, urlcon);
    
    
    /* define classes for Bytes URL Handler */
     
    urlh = (*env)->DefineClass(env, "org/endurox/loader/BytesURLHandler", loader, 
                                (const jbyte*) ndrx_G_resource_BytesURLHandler, 
                                ndrx_G_resource_BytesURLHandler_len_def);
    
    if(!urlh)
    {
        EXJLD_LOG_EXCEPTION(env, log_error, 
                            "Failed to define BytesURLHandler: %s");
        EXFAIL_OUT(ret);
    }
    
    urlh = (jclass) (*env)->NewGlobalRef(env, urlh);
    
    if(!urlh)
    {
        EXJLD_LOG_EXCEPTION(env, log_error, 
                            "Failed to NewGlobalRef BytesURLHandler: %s");
        EXFAIL_OUT(ret);
    }

    /* define classes for Bytes URL Handler */
    
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
    
    m[1].fnPtr = getClassBytes;
    m[1].name = "getClassBytes";
    m[1].signature = "(Ljava/lang/String;)[B";

    /* have these loader methods in both classes */
    (*env)->RegisterNatives(env, urlcon, m, 2);

    (*env)->RegisterNatives(env, M_classLoaderClass, m, 2);
    
    if((*env)->ExceptionCheck(env))
    {
        EXJLD_LOG_EXCEPTION(env, log_error, 
                            "Failed to register native methods: %s");
        EXFAIL_OUT(ret);
    }

    /* TODO: Create list of URLs for -cp -classpath and CLASSPATH env. 
     * Convert String List to object array
     */
    cpUrls = ndrxj_build_classpath_urls(env, M_classpath_url);
    
    if (NULL==cpUrls)
    {
        EXJLD_LOG_EXCEPTION(env, log_error, 
                            "Failed to create array classpath URLs: %s");
        EXFAIL_OUT(ret);
    }
    
    ldr_ctor = (*env)->GetMethodID(env, M_classLoaderClass, "<init>", "([Ljava/net/URL;)V");

    if(!ldr_ctor) 
    {
        EXJLD_LOG_EXCEPTION(env, log_error, 
                            "Failed to get static loader constructor: %s");
        EXFAIL_OUT(ret);
    }

    
    ldr_obj = (*env)->NewObject(env, M_classLoaderClass, ldr_ctor, cpUrls);

    if(!ldr_obj) 
    {
        EXJLD_LOG_EXCEPTION(env, log_error, 
                            "Cannot create loader instance: %s");
        NDRX_LOG(log_error, "Failed to create class loader object");
        EXFAIL_OUT(ret);
    }

    M_classLoader = (*env)->NewGlobalRef(env, ldr_obj);

    NDRX_LOG(log_info, "Static loader ready for boot...");
	
out:

    if (NULL!=cpUrls)
    {
        (*env)->DeleteLocalRef( env, cpUrls);
    }
        
    NDRX_LOG(log_debug, "%s returns %d", __func__, ret);

    return ret;
}

#if 0
/* save the original pointer... */
__thread void * old_malloc_hook;

static void *
my_malloc_hook (size_t size, const void *caller)
{
  void *result;
  long ldate, ltime, lusec;
  nstd_tls_t *tls;
  /* Restore all old hooks */
  __malloc_hook = old_malloc_hook;
  
  /* Call recursively */
  result = malloc (size);
  
  /* Save underlying hooks */
  old_malloc_hook = __malloc_hook;
  
  /* printf might call malloc, so protect it too. 
  printf ("%08ld:%06ld%06d: malloc (%d) returns %p\n", ldate, ltime, lusec, 
          (int)size, result);
  
  ndrx_get_dt_local(&ldate, &ltime, &lusec);
  */
  
  /*
  userlog("malloc (%d) returns %p\n",
          (int)size, result);
  */
  
  NDRX_LOG(log_error, "malloc (%d) returns %p", (int)size, result);
  
  /* print some log only if we have a Enduro/X TLS... */

  /* Restore our own hooks */
  __malloc_hook = my_malloc_hook;
  
  return result;
}

static void
my_init (void)
{
  old_malloc_hook = __malloc_hook;
  __malloc_hook = my_malloc_hook;
}


#endif

/**
 * We need to split up the source so that
 * - we create JVM
 * - get static method for current thread (assoc the context)
 * See this:
 * https://stackoverflow.com/questions/26256558/calling-jni-createjavavm-function-twice
 * so we have single env, but may be assoc'd with multiple threads, which have
 * proper context set?
 */


/**
 * Get static handler/factory method of the object
 * @param env java env
 * @param run_class class name to run
 * @param static_method method name to run
 * @param static_method_sign signature of the method
 * @param run_class ptr to class
 * @param run_mid ptr to method id
 * @return EXSUCCEED/EXFAIL
 */
expublic int ndrxj_ldr_get_static_handler(JNIEnv *env, 
			char *run_class_str,
                        char *static_method,
                        char *static_method_sign,
                        jclass *run_class,
                        jmethodID *run_mid
  			)
{
    int ret = EXSUCCEED;
    jmethodID load_class;
    jclass thread_class;
    jmethodID cur_thread_mid;
    jobject cur_thread;
    jmethodID set_ctx_mid;
    jstring jrun_class_str = NULL;

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
    jrun_class_str = (*env)->NewStringUTF(env, run_class_str);
    *run_class = (jclass) (*env)->CallObjectMethod(env, M_classLoader, 
                                                   load_class, 
                                                    jrun_class_str);
    if (NULL==*run_class)
    {
        EXJLD_LOG_EXCEPTION(env, log_error, 
                            "Failed to get main class: %s");
        EXFAIL_OUT(ret);
    }

    /* get main method... */

    *run_mid = (*env)->GetStaticMethodID(env, *run_class, static_method, 
                                    /*"([Ljava/lang/String;)V"*/static_method_sign); 

    if (*run_mid == 0)
    { 
        EXJLD_LOG_EXCEPTION(env, log_error, 
                            "Failed to get main method: %s");
        EXFAIL_OUT(ret);
    }

    
    /* set current class loader? */
    thread_class = (*env)->FindClass(env, "java/lang/Thread");
    
    if (NULL==thread_class)
    {
        EXJLD_LOG_EXCEPTION(env, log_error, 
                "Failed to find Thread class: %s");
        EXFAIL_OUT(ret);
    }
    
    cur_thread_mid = (*env)->GetStaticMethodID(env, thread_class, 
            "currentThread", "()Ljava/lang/Thread;");
    
    if (NULL==cur_thread_mid)
    {
        EXJLD_LOG_EXCEPTION(env, log_error, 
                "Failed to get currentThread() mid: %s");
        EXFAIL_OUT(ret);
    }
    
    cur_thread = (*env)->CallStaticObjectMethod(env, thread_class, cur_thread_mid);
    
    if (NULL==cur_thread)
    {
        EXJLD_LOG_EXCEPTION(env, log_error, 
                "Failed to get current thread: %s");
        EXFAIL_OUT(ret);
    }
    
    set_ctx_mid = (*env)->GetMethodID(env, thread_class, 
            "setContextClassLoader", "(Ljava/lang/ClassLoader;)V");
    
    if (NULL==set_ctx_mid)
    {
        EXJLD_LOG_EXCEPTION(env, log_error, 
                "Failed to get setContextClassLoader() mid: %s");
        EXFAIL_OUT(ret);
    }
    
    (*env)->CallVoidMethod(env, cur_thread, set_ctx_mid, M_classLoader);
    
    
    if((*env)->ExceptionCheck(env))
    {
        EXJLD_LOG_EXCEPTION(env, log_error, 
                        "Failed to set class loader when creating: %s (%s)",
                        run_class_str);
        (*env)->ExceptionClear(env);
        EXFAIL_OUT(ret);
    }
	
out:
    
    if (NULL!=thread_class)
    {
        (*env)->DeleteLocalRef( env, thread_class);
    }

    if (NULL!=jrun_class_str)
    {
        (*env)->DeleteLocalRef( env, jrun_class_str);
    }

    NDRX_LOG(log_debug, "%s returns  %d", __func__, ret);

    return ret;
}


/**
 * Run the loader 
 * For exjdbc we could call a static method
 * for allocating ATMI Context.
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
    jclass main_class = NULL;
    jclass str_class = NULL;
    jmethodID mid;
    jobjectArray args = NULL; 
    int i;

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

    args = (*env)->NewObjectArray(env, argc, str_class, 0); 

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
        
        /* delete local ref of string */
        
        if (NULL!=argString)
        {
            (*env)->DeleteLocalRef( env, argString);
        }
    }
    
    if (EXSUCCEED!=ndrxj_ldr_get_static_handler(env, 
			main_class_str,
                        "main",
                        "([Ljava/lang/String;)V",
                        &main_class,
                        &mid))
    {
        NDRX_LOG(log_error, "Failed to get main method for [%s] class", main_class_str);
        EXFAIL_OUT(ret);
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
            (*env)->ExceptionClear(env);
            EXFAIL_OUT(ret);
        }

    }
	
out:
    NDRX_LOG(log_debug, "%s returns  %d", __func__, ret);

    if (NULL!=main_class)
    {
        (*env)->DeleteLocalRef( env, main_class);
    }

    if (NULL!=str_class)
    {
        (*env)->DeleteLocalRef( env, str_class);
    }

    if (NULL!=args)
    {
        (*env)->DeleteLocalRef( env, args);
    }

    return ret;
}

/**
 * Get java virtual machine instance
 * NOTE ! Only single instance per process!
 * @param class_index array of classes
 * @param class_index_len array len
 * @param res_index resources array
 * @param res_index_len res array len
 * @param[out] env allocated env for current rhead
 * @return ptr to Java VM or NULL on failure
 */
expublic JavaVM * ndrxj_ldr_getvm(ndrxj_class_index_t *class_index, 
	        int class_index_len, ndrxj_class_index_t *res_index, int res_index_len,
                JNIEnv **env)
{
    JavaVM *vm = NULL;
    int ret = EXSUCCEED;
    JavaVMInitArgs vm_args;
    jint res;
    char *cp_env = NULL;
    ndrx_inicfg_t *cfg = NULL;
    ndrx_inicfg_section_keyval_t *out = NULL;
    ndrx_inicfg_section_keyval_t *val, *val_tmp;
    JavaVMOption *options = NULL;
    int n_opt = 0;
    
    NDRX_LOG(log_debug, "Configuring Java VM");
    
    
    M_class_index = class_index;
    M_class_index_len = class_index_len;
    
    M_res_index = res_index;
    M_res_index_len = res_index_len;
    
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
                /* TODO: Check the classpath if have any..
                 * this class path shall be stored in separate list
                 */
                
                /* check is it class path & load */
                if (EXSUCCEED!=ndrxj_cp_proc(&M_classpath_url,  val->val))
                {
                    NDRX_LOG(log_error, "Failed to process config string [%s] "
                            "for classpath", val->val);
                    EXFAIL_OUT(ret);
                }
                
            }
        }
    }
    
    /* Load classpath from env */
    cp_env = getenv("CLASSPATH");
    
    if (NULL!=cp_env)
    {
        if (EXSUCCEED!=ndrx_string_list_splitadd(&M_classpath_url, cp_env, ":"))
        {
            NDRX_LOG(log_error, "");
            EXFAIL_OUT(ret);
        }
    }

    vm_args.version = JNI_VERSION_1_2;
    vm_args.options = options;
    vm_args.nOptions = n_opt;
    vm_args.ignoreUnrecognized = 1;

    NDRX_LOG(log_debug, "Creating JVM...");
    res = JNI_CreateJavaVM(&vm, (void **)env, &vm_args);

    if (res < 0) 
    { 
        NDRX_LOG(log_error, "Failed to create Java VM");
        EXFAIL_OUT(ret);
    }

    if (EXSUCCEED!=create_loader(*env, vm))
    {
        NDRX_LOG(log_error, "Failed to prepare class loader");
        EXFAIL_OUT(ret);
    }
    
out:
    
    if((**env)->ExceptionCheck(*env))
    {
        EXJLD_LOG_EXCEPTION((*env), log_error, 
                        "Failed to create JVM Instance!: %s");
        EXFAIL_OUT(ret);
    }

    /* free up resources: */

    if (NULL!=M_classpath_url)
    {
        ndrx_string_list_free(M_classpath_url);
    }

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


    if (EXSUCCEED!=ret)
    {
        return NULL;
    }

    return vm;
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
    JavaVM *vm = NULL;
    JNIEnv *env = NULL;
    int ret = EXSUCCEED;
    
    vm=ndrxj_ldr_getvm(class_index, class_index_len, emb_index, emb_index_len,
            &env);
    
    if (NULL==vm)
    {
        NDRX_LOG(log_error, "Failed to create Java VM");
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
   
   /* terminate the VM? 
    * Do we get ever here?
    * Test the return status!
    */
   if (NULL!=vm)
   {
        (*vm)->DestroyJavaVM(vm);
   }
    
   return ret;
} 

/* vim: set ts=4 sw=4 et smartindent: */
