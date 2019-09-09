/**
 * @brief Resource management add/find duplicate/sort
 *
 * @file resources.c
 */
/* -----------------------------------------------------------------------------
 * Enduro/X Middleware Platform for Distributed Transaction Processing
 * Copyright (C) 2009-2016, ATR Baltic, Ltd. All Rights Reserved.
 * Copyright (C) 2017-2019, Mavimax, Ltd. All Rights Reserved.
 * This software is released under one of the following licenses:
 * LGPL or Mavimax's license for commercial use.
 * See LICENSE text.
 *
 * C (as designed by Dennis Ritchie and later authors) language code is licensed
 * under Enduro/X Modified GNU Affero General Public License, version 3.
 * See LICENSE_C text.
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
#include <nstdutil.h>

#include "exjld.h"

/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
/*---------------------------Statics------------------------------------*/
MUTEX_LOCKDECL(M_hash_lock);

/**
 * Find resource in the hash 
 * @param head hash head
 * @param resname resource name
 * @return NULL (not found) or ptr to object
 */
expublic exjld_resource_t * exljd_res_find(exjld_resource_t *head, char *resname)
{
    exjld_resource_t *ret;
    
    MUTEX_LOCK_V(M_hash_lock);
    EXHASH_FIND_STR( head, resname, ret);
    MUTEX_UNLOCK_V(M_hash_lock);
    
    return ret;
}

/**
 * Add resource to the hash. Check that record isn't duplicate. If so
 * return an error.
 * This function will also invoke embedded file generator
 
 * @return EXSUCCEED / EXFAIL
 */
void exljd_res_add_th (void *ptr, int *p_finish_off)
{
    int ret = EXSUCCEED;
    exjld_resource_t *elm;
    char cmd[PATH_MAX+1];
    resgen_thread_data_t *data = (resgen_thread_data_t *)ptr;
    
    if (NULL!=(elm=exljd_res_find(*data->head, data->resname)))
    {
        exjld_thread_debug_lock();
        NDRX_LOG(log_error, "Duplicate resource name: [%s] file: [%s] id: %d",
                data->resname, elm->respath, elm->id);
        exjld_thread_debug_unlock();
        EXFAIL_OUT(ret);
    }
    
    /* Allocate the element and add */
    if (NULL==(elm = NDRX_MALLOC(sizeof(exjld_resource_t))))
    {
        int err = errno;
        
        exjld_thread_debug_lock();
        NDRX_LOG(log_error, "Failed to malloc %d bytes: %s", 
                sizeof(exjld_resource_t), strerror(err));
        userlog("Failed to malloc %d bytes: %s", 
                sizeof(exjld_resource_t), strerror(err));
        exjld_thread_debug_unlock();
        EXFAIL_OUT(ret);
    }
    
    elm->id = data->id;
    
    NDRX_STRCPY_SAFE(elm->respath, data->respath);
    NDRX_STRCPY_SAFE(elm->resname, data->resname);
    
    snprintf(elm->embpath, sizeof(elm->embpath), "%s_%d", data->emb_pfx, data->id);
    
    /*
    snprintf(cmd, sizeof(cmd), "exembedfile '%s' %s cinclude", 
            elm->respath, elm->embpath);
    */
    
    
    /*
    exjld_thread_debug_lock();
    NDRX_LOG(log_dump, "%s", cmd);
    exjld_thread_debug_unlock();
    if (EXSUCCEED!=(ret = system(cmd)))
    {
        NDRX_LOG(log_error, "%s failed: %d", cmd, ret);
        EXFAIL_OUT(ret);
    }
    */
    
    if (EXSUCCEED!=ndrx_file_gen_embed(elm->respath, elm->embpath, "cinclude"))
    {
        NDRX_LOG(log_error, "Failed to generate resource (probably missing file "
                "or no permissions to read)");
        EXFAIL_OUT(ret);
    }
    
    MUTEX_LOCK_V(M_hash_lock);
    EXHASH_ADD_STR( (*data->head), resname, elm);
    MUTEX_UNLOCK_V(M_hash_lock);
    
out:
    
    if (EXSUCCEED!=ret && NULL!=elm)
    {
        NDRX_FREE(elm);
    }

    if (NULL!=data)
    {
        NDRX_FREE(data->resname);
        NDRX_FREE(data->respath);
        NDRX_FREE(data->emb_pfx);
        NDRX_FREE(data);
    }

    if (EXSUCCEED!=ret)
    {
        exjld_thread_error_set(ret);
    }

    return;
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
expublic void exljd_res_free(exjld_resource_t **head)
{
   exjld_resource_t *el, *elt;
   
   EXHASH_ITER(hh, *head, el, elt)
   {
        EXHASH_DEL(*head, el);
        NDRX_FREE(el);
   }
}

/* vim: set ts=4 sw=4 et smartindent: */
