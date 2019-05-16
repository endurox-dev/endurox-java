/**
 * @brief XA support routines for Enduro/X Transaction Manager Server (tmsrv)
 *
 * @file xatmsrv.c
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
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <ctype.h>
#include <errno.h>

#include <userlog.h>

#include <ndrstandard.h>
#include <ndebug.h>

#include <exparson.h>
#include <jni.h>
#include <ubf.h>

/*------------------------------Externs---------------------------------------*/
/*------------------------------Macros----------------------------------------*/
/*------------------------------Enums-----------------------------------------*/
/*------------------------------Typedefs--------------------------------------*/
/*------------------------------Globals---------------------------------------*/
/*------------------------------Statics---------------------------------------*/

/**
 * Virtual machine handler started for C processes / not loaded by java
 */
exprivate JavaVM *M_jvm = NULL;

/*------------------------------Prototypes------------------------------------*/

/**
 * Create VM.
 * Still no ATMI Context allocation. That will be done once actual XA calls
 * will be done.
 * The java env will be ignored.
 */
exprivate int ndrxj_jvm_create(void)
{
    
}

/**
 * Allocate ATMI Context object and install it in ATMI C Context
 * @return 
 */
exprivate int ndrxj_alloc_context(ndrx_ctx_priv_t *ctxpriv)
{
    int ret = EXSUCCEED;
    jobject jret = NULL;
    jclass bclz = NULL;
    jmethodID mid;
    TPCONTEXT_T ctx;
    
    bclz = (*NDRXJ_JENV(ctxpriv))->FindClass(NDRXJ_JENV(ctxpriv), ATMICTX_CLASS);
    
    if (NULL==bclz)
    {        
        /* I guess we need to abort here! */
        NDRX_LOG(log_error, "Failed to find AtmiCtx - aborting...!");
        /* tpreturn fail or simulate time-out? or just abort?*/
        abort();
    }
    
        /* create buffer object... */
    mid = (*NDRXJ_JENV(ctxpriv))->GetMethodID(NDRXJ_JENV(ctxpriv), bclz, "<init>", "(J)V");
    
    if (NULL==mid)
    {
        NDRX_LOG(log_error, "Cannot get buffer constructor!");
        goto out;
    }

    NDRX_LOG(log_debug, "About to NewObject(%s)", ATMICTX_CLASS);
    
    /* Install current context handler */
    
    /* get current atmi context */
    tpgetctxt(&ctx, 0L);
    tpsetctxt(ctx, 0L);
    
    jret = (*NDRXJ_JENV(ctxpriv))->NewObject(NDRXJ_JENV(ctxpriv), bclz, mid, (jlong)ctx);
    
    if (NULL==jret)
    {
        NDRX_LOG(log_error, "Failed to create [%s]", ATMICTX_CLASS);
        
        /* backtrace? */
        NDRXJ_LOG_EXCEPTION((NDRXJ_JENV(ctxpriv)), log_error, NDRXJ_LOGEX_ULOG, 
                "Failed to create Atmi Context: %s");
        
        goto out;
    }
    
    NDRX_LOG(log_debug, "NewObject() done - atmi ctx");
    
    /* create reference... */
    jret = (*NDRXJ_JENV(ctxpriv))->NewGlobalRef(NDRXJ_JENV(ctxpriv), jret);
    
    NDRXJ_CCTX_LVAL(ctxpriv) = ctx;
    NDRXJ_JATMICTX_LVAL(ctxpriv) = jret;

out:
    
    if (NULL!=bclz)
    {
        (*NDRXJ_JENV(ctxpriv))->DeleteLocalRef(NDRXJ_JENV(ctxpriv), bclz);
    }

    return ret;
}

/* vim: set ts=4 sw=4 et smartindent: */
