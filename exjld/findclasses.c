/**
 * @brief Contains routines for scanning the extracted class files in current
 *   and sub-directories. Build the array with files list and generates the
 *   resources.
 *
 * @file findclasses.c
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

#include <ndrx_config.h>

#ifndef EX_OS_DARWIN
#define _LARGEFILE64_SOURCE
#define _FILE_OFFSET_BITS 64 
#define _XOPEN_SOURCE 500
#endif

#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ftw.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <ftw.h>

#include <ndrstandard.h>
#include <ndebug.h>
#include "exjld.h"
/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
exprivate int M_was_file = EXFALSE; /**< Have we processed something?       */

/**
 * Process directory entry callback
 * @param filepath
 * @param info
 * @param typeflag
 * @param pathinfo
 * @return 
 */
exprivate int process_entry(const char *filepath, const struct stat *info,
                const int typeflag, struct FTW *pathinfo)
{
    int ret = EXSUCCEED;
    int len;
    static int id_class = 0;
    static int id_res = 0;
    char tmp[PATH_MAX];
    char *p = tmp;
    int i;
    int is_class = EXFALSE;
    resgen_thread_data_t *data;
    len = strlen(filepath);
    
    /* Accept files only... */
    if (FTW_F == typeflag /*&& len > 6  && 0==strcmp(filepath + (len - 6), ".class") */)
    {
        
        exjld_thread_debug_lock();
        NDRX_LOG(log_dump, "Processing as class: [%s]", filepath);
        exjld_thread_debug_unlock();
        
        
        if (len > 6 && 0==strcmp(filepath + (len - 6), ".class"))
        {
            id_class++;
            is_class = EXTRUE;
        }
        else
        {
            id_res++;
        }
        NDRX_STRCPY_SAFE(tmp, filepath);
        
        /* Extract class name (build from path and file name) */
                while (*p=='.' || *p=='/')
        {
            p++;
            len--;
        }
        
        /* replace dir sep with . */
        if (is_class)
        {
            /* transform to class path, for resources leave as is. */
            for (i=0; i<len; i++)
            {
                if ('/'==p[i])
                {
                    p[i]='.';
                }
            }
            
            /* strip down lass 5 symbols */
            p[len-6] = EXEOS;
        }
        
        exjld_thread_debug_lock();
        NDRX_LOG(log_dump, "Got class: [%s]", p);
        exjld_thread_debug_unlock();
        
        /* move to job... */
        /*
        if (EXSUCCEED!=exljd_res_add(&ndrx_G_classes_hash, p, id,  
                (char *)filepath, "class"))
        {
            NDRX_LOG(log_error, "Failed to add embedded resource [%s]", filepath);
            EXFAIL_OUT(ret);
        }
         * */
        
        data = NDRX_MALLOC(sizeof(resgen_thread_data_t));
        
        if (NULL==data)
        {
            NDRX_LOG(log_error, "Failed to malloc %d bytes: %s", 
                    sizeof(resgen_thread_data_t), strerror(errno));
            EXFAIL_OUT(ret);
        }
        
        
        /* TODO: is_class -> for class, others goes to resource files
         * update class loader to resolve resources via different function
         * and we would remove the -e key.
         */
        if (is_class)
        {
            /* fill up the call */
            data->head = &ndrx_G_classes_hash;
            data->id = id_class;
            NDRX_STRDUP_OUT(data->emb_pfx, "class");
        }
        else
        {
            data->head = &ndrx_G_emb_res_hash;
            data->id = id_res;
            NDRX_STRDUP_OUT(data->emb_pfx, "emb");
        }
        
        NDRX_STRDUP_OUT(data->resname, p);
        NDRX_STRDUP_OUT(data->respath, filepath);
        
        M_was_file = EXTRUE;
        
        thpool_add_work(ndrx_G_thpool, (void*)exljd_res_add_th, (void *)data);
        
    }
    
out:
    return ret;    
}

/**
 * Find the list of classes extract and put them into linear array
 */
expublic int exjld_class_build_hash(void)
{
    
    int ret = EXSUCCEED;
    
    if (EXSUCCEED!=nftw("./", process_entry, 20, FTW_PHYS))
    {
        NDRX_LOG(log_error, "Failed to scan local directory: %s",
                    strerror(errno));
        EXFAIL_OUT(ret);
    }
    
    if (M_was_file)
    {
        thpool_wait(ndrx_G_thpool);
        
        if (EXSUCCEED!=ndrx_G_thpool_error)
        {
            NDRX_LOG(log_error, "Failed to build classes!");
            EXFAIL_OUT(ret);
        }
    }

out:
    return ret;
}

/* vim: set ts=4 sw=4 et smartindent: */
