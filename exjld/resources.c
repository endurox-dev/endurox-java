/**
 * @brief Resource management add/find duplicate/sort
 *
 * @file resources.c
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
#include <stdlib.h>
#include <unistd.h>
#include <ftw.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <ndrstandard.h>
#include <ndebug.h>
#include <exhash.h>

#include "exjld.h"

/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/


/**
 * Find resource in the hash 
 * @param head hash head
 * @param resname resource name
 * @return NULL (not found) or ptr to object
 */
expublic exjld_resource_t * exljd_res_find(exjld_resource_t *head, char *resname)
{
    exjld_resource_t *ret;
    EXHASH_FIND_STR( head, resname, ret);
    return ret;
}

/**
 * Add resource to the hash. Check that record isn't duplicate. If so
 * return an error.
 * This function will also invoke embedded file generator
 * @param head hash head (double ptr as can change e.g. from NULL to value)
 * @param resname resource name
 * @param id resource id (just unique id for resource files)
 * @param respath resource path on disk
 * @param emb_pfx embedded file prefix - either class or resources..
 * @return EXSUCCEED / EXFAIL
 */
expublic int exljd_res_add(exjld_resource_t **head, char *resname,
        int id, char *respath, char *emb_pfx)
{
    int ret = EXSUCCEED;
    exjld_resource_t *elm;
    char cmd[PATH_MAX+1];
    
    if (NULL!=(elm=exljd_res_find(*head, resname)))
    {
        NDRX_LOG(log_error, "Duplicate resource name: [%s] file: [%s] id: %d",
                resname, elm->respath, elm->id);
        EXFAIL_OUT(ret);
    }
    
    /* Allocate the element and add */
    if (NULL==(elm = NDRX_MALLOC(sizeof(exjld_resource_t))))
    {
        int err = errno;
        
        NDRX_LOG(log_error, "Failed to malloc %d bytes: %s", 
                sizeof(exjld_resource_t), strerror(err));
        userlog("Failed to malloc %d bytes: %s", 
                sizeof(exjld_resource_t), strerror(err));
        EXFAIL_OUT(ret);
    }
    
    elm->id = id;
    
    NDRX_STRCPY_SAFE(elm->respath, respath);
    NDRX_STRCPY_SAFE(elm->resname, resname);
    
    snprintf(elm->embpath, sizeof(elm->embpath), "%s_%d", emb_pfx, id);
    snprintf(cmd, sizeof(cmd), "exembedfile '%s' %s cinclude", 
            elm->respath, elm->embpath);
    
    NDRX_LOG(log_dump, "%s", cmd);
    fprintf(stderr, "%s\n", cmd);
    if (EXSUCCEED!=(ret = system(cmd)))
    {
        NDRX_LOG(log_error, "%s failed: %d", cmd, ret);
        EXFAIL_OUT(ret);
    }
    
    EXHASH_ADD_STR( *head, resname, elm);
    
out:
    
    if (EXSUCCEED!=ret && NULL!=elm)
    {
        NDRX_FREE(elm);
    }
    
    return ret;
}

/**
 * Sort by name
 * @param a
 * @param b
 * @return -1, 0, 1
 */
exprivate int resname_sort(exjld_resource_t *a, exjld_resource_t *b) 
{
    return strcmp(a->resname,b->resname);
}

/**
 * Sort hash list by resource name
 * @param head double ptr to hash head
 */
expublic void exljd_res_sort_by_resname(exjld_resource_t **head)
{
    EXHASH_SORT(*head, resname_sort);
}

/**
 * Free up resources taken by hash
 * @param head
 */ 
expublic void exljd_res_sort_by_free(exjld_resource_t **head)
{
   exjld_resource_t *el, *elt;
   
   EXHASH_ITER(hh, *head, el, elt)
   {
        EXHASH_DEL(*head, el);
        NDRX_FREE(el);
   }
}

/* vim: set ts=4 sw=4 et smartindent: */
