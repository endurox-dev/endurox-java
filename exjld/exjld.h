/**
 * @brief Enduro/X Java Linker internal header
 *
 * @file exjld.h
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
#ifndef EXJLD_H_
#define EXJLD_H_
/*------------------------------Includes--------------------------------------*/
#include <sys_unix.h>
#include <exhash.h>
/*------------------------------Macros----------------------------------------*/
/*------------------------------Enums-----------------------------------------*/
/*------------------------------Typedefs--------------------------------------*/

/**
 * Compiled resource either class or embedded resource, struct
 */
struct exjld_resource
{
    
    /** resource name */
    char resname[PATH_MAX+1];
    
    /** resource ID */
    int id;
    
    /** real path to file */
    char respath[PATH_MAX+1];
    
    /** path to embedded file */
    char embpath[PATH_MAX+1];
    
    /** makes this structure hashable */
    EX_hash_handle hh;
};

/**
 * Compiled resource either class or embedded resource, type
 */
typedef struct exjld_resource exjld_resource_t;

/*------------------------------Externs---------------------------------------*/
extern char ndrx_G_build_cmd[];
extern int ndrx_G_do_test;
extern char ndrx_G_main_class[];
extern char ndrx_G_out_bin[];
extern char ndrx_G_wd[];
extern char ndrx_G_owd[];
extern string_list_t* ndrx_G_libpath;
extern string_list_t* ndrx_G_libs;
extern int ndrx_G_keep_temp;
extern string_list_t* ndrx_G_embedded_res;

extern exjld_resource_t *ndrx_G_classes_hash;
extern exjld_resource_t *ndrx_G_emb_res_hash;

/*------------------------------Globals---------------------------------------*/
/*------------------------------Statics---------------------------------------*/
/*------------------------------Prototypes------------------------------------*/


extern exjld_resource_t * exljd_res_find(exjld_resource_t *head, char *resname);
extern int exljd_res_add(exjld_resource_t **head, char *resname,
        int id, char *respath, char *emb_pfx);
extern void exljd_res_sort_by_resname(exjld_resource_t **head);
extern void exljd_res_sort_by_free(exjld_resource_t **head);


extern int exjld_emb_build_hash(void);


#endif /* EXJLD_H_ */
