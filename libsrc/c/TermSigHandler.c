/**
 * @brief Handle process termination
 *
 * @file TermSigHandler.c
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
#include <signal.h>
#include <thlock.h>
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/

/* have some locking if doing access from two threads... */

/**
 * Is library initialized?
 */
exprivate int M_is_set = EXFALSE;
exprivate pthread_t M_signal_thread; /* Signalled thread */
MUTEX_LOCKDECL(M_is_set_lock) static pthread_mutex_t X = PTHREAD_MUTEX_INITIALIZER;

/** Needs handler to VM: */
exprivate JavaVM *M_vm = NULL;
exprivate jobject M_runner = NULL; /**< Thread to which delivery notification */
/*---------------------------Prototypes---------------------------------*/

/**
 * Thread for shutdown monitor
 * The signals are disabled and not passed to java anymore
 * otherwise it causes some issues and corruptions during java/jni processing
 * @param arg not used
 * @return  not used
 */
exprivate void * sig_thread(void *arg)
{
    sigset_t blockMask;
    JNIEnv *env = NULL;
    int sig;
    TPCONTEXT_T ctx;
    
    /* have some context for logging, etc.. */
    
    ctx = tpnewctxt(0, 1);
    
    
    sigemptyset(&blockMask);
    sigaddset(&blockMask, SIGTERM);
    sigaddset(&blockMask, SIGINT);
    sigaddset(&blockMask, SIGHUP);
    
    
    if (pthread_sigmask(SIG_BLOCK, &blockMask, NULL) == -1)
    {
        NDRX_LOG(log_always, "%s: pthread_sigmask failed (thread): %s",
            __func__, strerror(errno));
    }
    
    /* Get the java env for given thread */
    /*
    if (EXSUCCEED!=((*M_vm)->GetEnv(M_vm, (void **)&env, JNI_VERSION_1_6)))
    {
        NDRX_LOG(log_error, "Failed to get java env - terminate!");
        userlog("Failed to get java env - terminate!");
        exit(EXFAIL);
    }*/
    
    if (EXSUCCEED!=(*M_vm)->AttachCurrentThreadAsDaemon(M_vm, (void **)&env, NULL))
    {
        NDRX_LOG(log_error, "Failed to AttachCurrentThreadAsDaemon - terminate!");
        userlog("Failed to AttachCurrentThreadAsDaemon - terminate!");
        exit(EXFAIL);
    }
    
    /* wait for signal... */
    
    for (;;)
    {
        /* Wait for notification signal  */
        NDRX_LOG(log_debug, "Ok, waiting shutdown signal...");
        
        if (EXSUCCEED!=sigwait(&blockMask, &sig))     
        {
            NDRX_LOG(log_warn, "sigwait failed:(%s)", strerror(errno));
        }
        
        NDRX_LOG(log_error, "Got signal: %d - notify java", sig);
        
        /**
         * WARNING ! The java method must not change the shutdown handlers!
         */
        MUTEX_LOCK_V(M_is_set_lock);
        /* unset ATMI CTX */
        tpsetctxt(TPNULLCONTEXT, 0L);
        
        if (NULL!=M_runner)
        {
            (*env)->CallObjectMethod(env, M_runner, ndrxj_clazz_Runnable_mid_run);
        }
        
        tpsetctxt(ctx, 0L);
        MUTEX_UNLOCK_V(M_is_set_lock);
        
    }
    
    NDRX_LOG(log_debug, "Shutdown mon terminates...");
    
    return NULL;
}

/**
 * Restart handler for JNI
 * Migth be on any thread
 * @param sig
 */
void sig_handler(int sig)
{
    /* wakup man thread... */
    pthread_kill(M_signal_thread, SIGTERM);
}

/**
 * Install and configuration termination handler
 * Firstly this will boot separate thread, 
 * @param env java env
 * @param atmiCtxObj Atmi context
 * @param runner Runner object
 */
expublic NDRX_JAVA_API void JNICALL ndrxj_Java_org_endurox_AtmiCtx_installTermSigHandler
        (JNIEnv *env, jclass atmiCtxObj, jobject runner)
{
    int ret = EXSUCCEED;
    int have_lock = EXFALSE;
    pthread_attr_t pthread_custom_attr;
    
    /* set ctx from obj */
    if (NULL == ndrxj_get_ctx(env, atmiCtxObj, EXTRUE))
    {
        return;
    }
    
    if (!M_is_set)
    {
        MUTEX_LOCK_V(M_is_set_lock);
        have_lock = EXTRUE;
        
        if (!M_is_set)
        {
            /* do not want signals... was we will wait for pid
             * in threaded and non threaded mode.
             */
            
            static struct sigaction act;

            memset(&act, 0, sizeof(act));
            act.sa_handler = sig_handler;
            act.sa_flags = SA_RESTART;

            sigaction(SIGTERM, &act, NULL);
            sigaction(SIGINT, &act, NULL);
            sigaction(SIGHUP, &act, NULL);
    
            
            if (EXSUCCEED!=(*env)->GetJavaVM(env, &M_vm))
            {
                NDRX_LOG(log_error, "Failed to get Java VM!");
                ndrxj_atmi_throw(env, NULL, NULL, TPESYSTEM, "Failed to get VM!");
                EXFAIL_OUT(ret);
            }
            
            /* set default action for signals too... */
            
            pthread_attr_init(&pthread_custom_attr);
            /* set some small stacks size, 1M should be fine! */
            ndrx_platf_stack_set(&pthread_custom_attr);
            
            /* start the thread for monitoring signals... */
            
            pthread_create(&M_signal_thread, &pthread_custom_attr, 
                    sig_thread, NULL);
            
            NDRX_LOG(log_info, "Shutdown monitoring thread installed...");

        }
    }
    
    if (!have_lock)
    {
        MUTEX_LOCK_V(M_is_set_lock);
    }
    
    /* create reference, if have one already, delete */
    
    if (NULL!=M_runner)
    {
        (*env)->DeleteGlobalRef(env, M_runner);
    }
    
    if (NULL!=runner)
    {
        M_runner = (*env)->NewGlobalRef(env, runner);
    }
    else
    {
        M_runner = NULL;
    }
            
    
    NDRX_LOG(log_info, "Shutdown runner installed...");

out:
    
    if (have_lock)
    {
        MUTEX_UNLOCK_V(M_is_set_lock);
    }

    tpsetctxt(TPNULLCONTEXT, 0L);
}

/* vim: set ts=4 sw=4 et smartindent: */
