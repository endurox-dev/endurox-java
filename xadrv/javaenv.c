/**
 * @brief Create java environment for tmsrv case
 *
 * @file javaenv.c
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
#include <exjdbc.h>
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
/*---------------------------Prototypes---------------------------------*/

/**
 * Create java environment for tmsrv
 */
expublic int ndrxj_jvm_create(void)
{
    
    int ret = EXSUCCEED;
    JavaVM *vm = NULL;
    JavaVMInitArgs vm_args;
    jint res;
    ndrx_inicfg_t *cfg = NULL;
    ndrx_inicfg_section_keyval_t *out = NULL;
    ndrx_inicfg_section_keyval_t *val, *val_tmp;
    int n_opt = 0;

    /* should be allocated... */
    JavaVMOption *options = NULL;

    NDRX_LOG(log_debug, "Creating java env...");
    
    /* my_init(); */
    
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
    res = JNI_CreateJavaVM(&vm, (void **)&ndrxj_G_env, &vm_args);

    if (res < 0) 
    { 
        NDRX_LOG(log_error, "Failed to create Java VM");
        EXFAIL_OUT(ret);
    }
    
    /* start the dummy method... just to load classes */
    
out:
    return ret;

}

/* vim: set ts=4 sw=4 et smartindent: */
