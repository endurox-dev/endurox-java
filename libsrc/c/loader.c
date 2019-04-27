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
#include <thlock.h>

#include <tmenv.h>
#include <libsrc.h>

#include <xa.h>
#define __USE_GNU
#include <dlfcn.h>
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/

/**
 * Symbol mapping table
 */
struct ndrxj_loader_map
{
    char    *symbol;        /**< Symbol name                                  */
    void    **funcptr;      /**< Pointer to function                          */
};
typedef struct ndrxj_loader_map ndrxj_loader_map_t;


/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/

/* resolved funcs, generated list: */
exprivate jlong (*p_ndrxj_Java_org_endurox_AtmiCtx_tpnewctxt)
        (JNIEnv *, jclass) = NULL;

exprivate jint (*p_ndrxj_Java_org_endurox_AtmiCtx_tplogqinfo) 
        (JNIEnv *, jobject, jint, jlong) = NULL;

exprivate void (*p_ndrxj_Java_org_endurox_AtmiCtx_tplogC)(JNIEnv *, jobject, jint, 
        jstring, jlong, jstring);

/**
 * Function mapping table
 */
exprivate ndrxj_loader_map_t M_funcmap[] =
{  
    /* generated list of maps: */
    {"ndrxj_Java_org_endurox_AtmiCtx_tpnewctxt", (void *)&p_ndrxj_Java_org_endurox_AtmiCtx_tpnewctxt},
    {"ndrxj_Java_org_endurox_AtmiCtx_tplogqinfo", (void *)&p_ndrxj_Java_org_endurox_AtmiCtx_tplogqinfo},
    {"ndrxj_Java_org_endurox_AtmiCtx_tplogC", (void *)&p_ndrxj_Java_org_endurox_AtmiCtx_tplogC},
    {NULL, NULL}
};
    
/**
 * Set to true if library is initialize / ptrs resolved
 */
exprivate int volatile M_lib_init = EXFALSE;
exprivate void * M_handle = NULL;

/**
 * Lock for init
 */
MUTEX_LOCKDECL(M_lib_init_lock);

/*---------------------------Prototypes---------------------------------*/

/**
 * Throw exception
 * @param env java env
 * @param msg message
 */
exprivate void ndrxj_lite_exception(JNIEnv *env, char *msg)
{
    (*env)->ThrowNew(env, (*env)->FindClass(env, "java/lang/Exception"), msg);
}

/**
 * Perform library init with mutex lock to avoid twice init - protect from
 * threaded access
 * @param[in] env Java env
 * @return EXSUCCEED/EXFAIL
 */
exprivate int ndrxj_lib_init(JNIEnv *env)
{
    int ret = EXSUCCEED;
    char buf[512];
#if EX_OS_DARWIN   
    char *lib = "libexjava.dylib";
#else
    char *lib = "libexjava.so";
#endif
    int i;
    
    MUTEX_LOCK_V(M_lib_init_lock);
    
    if (!M_lib_init)
    {
        /* check the symbol, if have one, then load directly
         * if does not have symbol, then load the library
         * Symbol mapping shall be done in separate function.
         */
        
        if (NULL==dlsym( RTLD_DEFAULT, "ndrxj_Java_org_endurox_AtmiCtx_tpnewctxt" ))
        {
            /* load shared library */
            M_handle = dlopen (lib, RTLD_NOW | RTLD_GLOBAL);
            
            if (!M_handle)
            {
                snprintf(buf, sizeof(buf), "Failed to load %s: %s", lib, dlerror());
                ndrxj_lite_exception(env, buf);
                EXFAIL_OUT(ret);
            }
        }
        
        /* load mappings, all maps shall succeed, else we give error */
        for (i=0; NULL!=M_funcmap[i].symbol; i++)
        {
            (*M_funcmap[i].funcptr) = dlsym(M_handle, M_funcmap[i].symbol);
            
            if (NULL==(*M_funcmap[i].funcptr))
            {
                snprintf(buf, sizeof(buf), "Failed to resolve `%s' function", 
                        M_funcmap[i].symbol);
                ndrxj_lite_exception(env, buf);
                EXFAIL_OUT(ret);
            }
        }
    }
    
out:
    MUTEX_UNLOCK_V(M_lib_init_lock);
    
    return ret;
}

/**
 * All Enduro/X Ops requires Atmi Context, thus
 * catch the call and load symbols globally...
 */
jlong JNICALL Java_org_endurox_AtmiCtx_tpnewctxt (JNIEnv *v1, jclass v2)
{
    if (!M_lib_init)
    {
        if (EXSUCCEED!=ndrxj_lib_init(v1))
        {
            return (jlong)0;
        }
    }
    
    return p_ndrxj_Java_org_endurox_AtmiCtx_tpnewctxt(v1, v2);
}

/**
 * All Enduro/X Ops requires Atmi Context, thus
 * catch the call and load symbols globally...
 */
void Java_org_endurox_AtmiCtx_tplogC(JNIEnv* v1, jobject v2, jint v3, 
        jstring v4, jlong v5, jstring v6)
{
    if (!M_lib_init)
    {
        if (EXSUCCEED!=ndrxj_lib_init(v1))
        {
            return;
        }
    }
    
    p_ndrxj_Java_org_endurox_AtmiCtx_tplogC(v1, v2, v3, v4, v5, v6);
}


#undef __USE_GNU
/* vim: set ts=4 sw=4 et smartindent: */
