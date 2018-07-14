/**
 * @brief Process embedded resources
 *
 * @file emb.c
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
#include <stdlib.h>
#include <unistd.h>
#include <ftw.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

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
 * Build hash of embedded resources
 */
expublic int exjld_emb_build_hash(void)
{
    int ret = EXSUCCEED;
    char path[PATH_MAX+1];
    int id=0;
    string_list_t* cur = ndrx_G_embedded_res;
    
    while (NULL!=cur)
    {
        id++;
        
        if (cur->qname[0] == '/')
        {
            NDRX_STRCPY_SAFE(path, cur->qname);
        }
        else
        {
            /* so it is relative path, use org dir as prefix */
            snprintf(path, sizeof(path), "%s/%s", ndrx_G_owd, path);
        }
        
        if (EXSUCCEED!=exljd_res_add(&ndrx_G_emb_res_hash, cur->qname, id, 
                path, "emb"))
        {
            NDRX_LOG(log_error, "Failed to add embedded resource [%s]", path);
            EXFAIL_OUT(ret);
        }
        
        cur=cur->next;
    }
out:
    
   return ret; 
}

/* vim: set ts=4 sw=4 et cindent: */
