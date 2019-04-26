/**
 * @brief This is wrapper for libexjava.so so that we expose loaded symbols
 *  globally. Otherwise XA drivers from SO cannot access the enduro/x symbols.
 *
 * @file loader.c
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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <ndrstandard.h>
#include <ndebug.h>
#include <atmi.h>
#include <sys_mqueue.h>

#include <tmenv.h>
#include <libsrc.h>

#include <xa.h>
#define __USE_GNU
#include <dlfcn.h>
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/

/**
 * All Enduro/X Ops requires Atmi Context, thus
 * catch the call and load symbols globally...
 */
jlong JNICALL Java_org_endurox_AtmiCtx_tpnewctxt (JNIEnv *env, jclass cls)
{
    static int loaded = EXFALSE;
    static jlong (*fpFunc)(JNIEnv *env, jclass cls) = NULL;
    jlong ctx = 0;
    void *handle = NULL;
    
    /* use locked loder! */
    if (!loaded)
    {
        
        /* TODO: Have ptr to func: jlong JNICALL Java_org_endurox_AtmiCtx_tpnewctxt (JNIEnv *env, jclass cls) */
        /* Loading the symbol... */
        handle = dlopen ("libexjava.so", RTLD_NOW | RTLD_GLOBAL);
        if (!handle)
        {
            fprintf(stderr, "Failed to load libexjava.so: %s\n", dlerror());
            ctx = 0;
            goto out;
        }
        else
        {
            loaded = EXTRUE;
        }
        
        /* get the symbol */
        if (NULL==(fpFunc = dlsym( handle, "Java_org_endurox_AtmiCtx_tpnewctxt_int" )))
        {
            fprintf(stderr, "Failed to resolve: Java_org_endurox_AtmiCtx_tpnewctxt_int: %s\n", 
                    dlerror());
            ctx = 0;
            goto out;
        }
        
        fprintf(stderr, "YOPT !Loaded!!!!!");
    }
    
    /* call the symbol */
    ctx = fpFunc (env, cls);
    
out:
    
    if (ctx = 0 && NULL!=handle)
    {
        dlclose(handle);
    }

    return (long)ctx;
}




#undef __USE_GNU
/* vim: set ts=4 sw=4 et smartindent: */
