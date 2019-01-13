/**
 * @brief Process embedded resources
 *
 * @file emb.c
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
    resgen_thread_data_t *data;
    int was_res = EXFALSE;
    
    while (NULL!=cur)
    {
        was_res = EXTRUE;
        id++;
        
        if (cur->qname[0] == '/')
        {
            NDRX_STRCPY_SAFE(path, cur->qname);
        }
        else
        {
            /* so it is relative path, use org dir as prefix */
            snprintf(path, sizeof(path), "%s/%s", ndrx_G_owd, cur->qname);
        }
        
        data = NDRX_MALLOC(sizeof(resgen_thread_data_t));
        
        if (NULL==data)
        {
            NDRX_LOG(log_error, "Failed to malloc %d bytes: %s", 
                    sizeof(resgen_thread_data_t), strerror(errno));
            EXFAIL_OUT(ret);
        }
        
        /* fill up the call */
        data->head = &ndrx_G_emb_res_hash;
        data->id = id;
        
        NDRX_STRDUP_OUT(data->resname, cur->qname);
        NDRX_STRDUP_OUT(data->respath, path);
        NDRX_STRDUP_OUT(data->emb_pfx, "emb");
        
        thpool_add_work(ndrx_G_thpool, (void*)exljd_res_add_th, (void *)data);
        
        cur=cur->next;
    }
    
    if (was_res)
    {
        thpool_wait(ndrx_G_thpool);
        
        if (EXSUCCEED!=ndrx_G_thpool_error)
        {
            NDRX_LOG(log_error, "Failed to build resources!");
            EXFAIL_OUT(ret);
        }
    }
    
out:
    
   return ret; 
}

/* vim: set ts=4 sw=4 et smartindent: */
