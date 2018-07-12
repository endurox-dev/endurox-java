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
/*------------------------------Externs---------------------------------------*/
extern char ndrx_G_build_cmd[];
extern int ndrx_G_do_test;
extern char ndrx_G_main_class[];
extern char ndrx_G_out_bin[];
extern char ndrx_G_wd[];
extern char ndrx_G_owd[];
extern string_list_t* ndrx_G_libpath;
extern string_list_t* ndrx_G_libs;
extern string_list_t* ndrx_G_embedded_res;
extern int ndrx_G_keep_temp;
/*------------------------------Macros----------------------------------------*/
/*------------------------------Enums-----------------------------------------*/
/*------------------------------Typedefs--------------------------------------*/

struct exjld_resource
{
    /** resource ID */
    int id;
    
    /** path to resource */
    char respath[PATH_MAX];
    
    /** resource name */
    char resname[PATH_MAX];
    
};

typedef struct exjld_resource exjld_resource_t;

/*------------------------------Globals---------------------------------------*/
/*------------------------------Statics---------------------------------------*/
/*------------------------------Prototypes------------------------------------*/

#endif /* EXJLD_H_ */
