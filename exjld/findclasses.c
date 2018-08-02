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
 * Copyright (C) 2017-2018, Mavimax, Ltd. All Rights Reserved.
 * This software is released under one of the following licenses:
 * GPL or Mavimax's license for commercial use.
 * -----------------------------------------------------------------------------
 * GPL license:
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 3 of the License, or (at your option) any later
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
#define _LARGEFILE64_SOURCE
#define _FILE_OFFSET_BITS 64 
#define _XOPEN_SOURCE 500

#include <stdlib.h>
#include <unistd.h>
#include <ftw.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
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
    static int id = 0;
    char tmp[PATH_MAX];
    char *p = tmp;
    int i;
    
    len = strlen(filepath);
    
    /* Accept files only... */
    if (FTW_F == typeflag && len > 6 && 0==strcmp(filepath + (len - 6), ".class"))
    {
        id++;
        NDRX_LOG(log_debug, "Processing as class: [%s]", filepath);
        
        NDRX_STRCPY_SAFE(tmp, filepath);
        
        /* Extract class name (build from path and file name) */
                while (*p=='.' || *p=='/')
        {
            p++;
            len--;
        }
        
        /* replace dir sep with . */
        for (i=0; i<len; i++)
        {
            if ('/'==p[i])
            {
                p[i]='.';
            }
        }
        
        /* strip down lass 5 symbols */
        p[len-6] = EXEOS;
        
        NDRX_LOG(log_debug, "Got class: [%s]", p);
        
        if (EXSUCCEED!=exljd_res_add(&ndrx_G_classes_hash, p, id,  
                (char *)filepath, "class"))
        {
            NDRX_LOG(log_error, "Failed to add embedded resource [%s]", filepath);
            EXFAIL_OUT(ret);
        }
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

out:
    return ret;
}

/* vim: set ts=4 sw=4 et cindent: */
