/**
 * @brief Enduro/X Java Linker internal header
 *
 * @file exjld.h
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
#ifndef EXJLD_H_
#define EXJLD_H_
/*------------------------------Includes--------------------------------------*/
/* #include <sys_unix.h> */
#include <exhash.h>
#include <exthpool.h>
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


/**
 * Structure for threaded resource generator
 */
typedef struct 
{
    /** head hash head (double ptr as can change e.g. from NULL to value) */
    exjld_resource_t **head;
    
    /** resource name */
    char *resname;
    /** resource id (just unique id for resource files) */
    int id;
    /** resource path on disk */
    char *respath;
    /** embedded file prefix - either class or resources.. */
    char *emb_pfx;
    
} resgen_thread_data_t;

/*------------------------------Externs---------------------------------------*/
extern char ndrx_G_build_cmd[];
extern int ndrx_G_do_test;
extern char ndrx_G_main_class[];
extern char ndrx_G_out_bin[];
extern char ndrx_G_wd[];
extern char ndrx_G_owd[];
extern string_list_t* ndrx_G_libpath;
extern string_list_t* ndrx_G_incpath;
extern string_list_t* ndrx_G_libs;
extern int ndrx_G_keep_temp;
extern string_list_t* ndrx_G_embedded_res;

extern exjld_resource_t *ndrx_G_classes_hash;
extern exjld_resource_t *ndrx_G_emb_res_hash;
extern int ndrx_G_thpool_error;
extern threadpool ndrx_G_thpool;

/*------------------------------Globals---------------------------------------*/
/*------------------------------Statics---------------------------------------*/
/*------------------------------Prototypes------------------------------------*/


extern exjld_resource_t * exljd_res_find(exjld_resource_t *head, char *resname);
extern void exljd_res_add_th (void *ptr, int *p_finish_off);

extern void exljd_res_sort_by_resname(exjld_resource_t **head);
extern void exljd_res_free(exjld_resource_t **head);

extern void exjld_thread_error_set(int ret);
extern void exjld_thread_debug_lock(void);
extern void exjld_thread_debug_unlock(void);

extern int exjld_emb_build_hash(void);
extern int exjld_class_build_hash(void);
extern int ndrxj_codegen(int test_mode);

#endif /* EXJLD_H_ */
/* vim: set ts=4 sw=4 et smartindent: */
