/**
 * @brief Generate code for the embedded binary
 *
 * @file codegen.c
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
 * Generate code for the new binary and execute test mode if needed
 * @param test_mode should we test main class?
 * @return EXSUCCEED/EXFAIL
 */
expublic int ndrxj_codegen(int test_mode)
{
    int ret = EXSUCCEED;
    FILE *f = NULL;
    exjld_resource_t *el, *elt;
    int first;
    int num_class = 0;
    int num_emb = 0;
    char buildcmd[PATH_MAX+1];
    char libs[PATH_MAX+1];
    char tmp[PATH_MAX+1];
    char include_path[PATH_MAX+1];
    char libpath_path[PATH_MAX+1];
    expublic string_list_t* iter;
#define FILENAME "jmain.c"
    
    if (NULL==(f=NDRX_FOPEN(FILENAME, "w")))
    {
        NDRX_LOG(log_error, "Failed to open for write [%s]: %s", 
                FILENAME, strerror(errno));
        EXFAIL_OUT(ret);
    }
    
    /* lets print out the code now... */
    
    fprintf(f, "#include <exjlds.h>\n");
    fprintf(f, "#include <stdio.h>\n");

    /* lets include all classes */
    
    EXHASH_ITER(hh, ndrx_G_classes_hash, el, elt)
    {
        fprintf(f, "#include \"%s.cinclude\"\n", el->embpath);
    }
    
    /* lets include all resources */
    
    EXHASH_ITER(hh, ndrx_G_emb_res_hash, el, elt)
    {
        fprintf(f, "#include \"%s.cinclude\"\n", el->embpath);
    }

    /* lets register classes */
    
    fprintf(f, "ndrxj_class_index_t ndrxj_G_class_index [] = \n");
    fprintf(f, "{\n");
    
    first = EXTRUE;
    EXHASH_ITER(hh, ndrx_G_classes_hash, el, elt)
    {
        num_class++;
        
        if (first)
        {
            first = EXFALSE;
        }
        else
        {
            fprintf(f, "	,\n");
        }
        fprintf(f, "	{\n");
        fprintf(f, "		\"%s\",\n", el->resname);
        fprintf(f, "		ndrx_G_resource_class_%d,\n", el->id);
        fprintf(f, "		ndrx_G_resource_class_%d_len_def\n", el->id);
        fprintf(f, "	}\n");
    }
    fprintf(f, "};\n\n");

    /* lets register resources  */
    first = EXTRUE;
    EXHASH_ITER(hh, ndrx_G_emb_res_hash, el, elt)
    {
        num_emb++;
        
        if (first)
        {
            fprintf(f, "ndrxj_class_index_t ndrxj_G_emb_index [] = \n");
            fprintf(f, "{\n");
            first=EXFALSE;
        }
        else
        {
            fprintf(f, "	,\n");
        }
        
        fprintf(f, "	{\n");
        fprintf(f, "		\"%s\",\n", el->resname);
        fprintf(f, "		ndrx_G_resource_emb_%d,\n", el->id);
        fprintf(f, "		ndrx_G_resource_emb_%d_len_def\n", el->id);
        fprintf(f, "	}\n");
    }
    
    if (!first)
    {
        fprintf(f, "};\n\n");
    }

    /* lets generate the main */
    fprintf(f, "int main(int argc, char **argv)\n");
    fprintf(f, "{\n");
    fprintf(f, "        \n");
    fprintf(f, "	return ndrxj_run_main(argc, argv, \"%s\",\n", ndrx_G_main_class);
    
    fprintf(f, "		ndrxj_G_class_index, %d, %s, %d,\n",
            num_class, (num_emb > 0? "ndrxj_G_emb_index":"NULL"),
            num_emb);
    
    fprintf(f, "                %d);\n", test_mode);
    fprintf(f, "}\n");
    
    
    /* finish off the file */
    NDRX_FCLOSE(f);
    f = NULL;
    /* and now build off:
     * 
     * 
     * buildserver -n -o a -f 'exj.c -lexjlds -ljava -ljvm' -a "-I ../../include/ -L../../libexjlds -L /usr/lib/jvm/java-8-openjdk-amd64/jre/lib/amd64 -L/usr/lib/jvm/java-8-openjdk-amd64/jre/lib/amd64/server/"
     *  
     */
    
    /* Now build the command...! */
    
    
    
    /*
    char libs[PATH_MAX+1];
    char include_path[PATH_MAX+1];
    char libpath_path[PATH_MAX+1];
    */
    
    libs[0]=EXEOS;
    iter = ndrx_G_libs;
    
    first = EXTRUE;
    while (iter)
    {
        if (first)
        {
            NDRX_STRCAT_S(libs, sizeof(libs), "-l");
            NDRX_STRCAT_S(libs, sizeof(libs), iter->qname);
            first=EXFALSE;
        }
        else
        {
            NDRX_STRCAT_S(libs, sizeof(libs), " ");
            NDRX_STRCAT_S(libs, sizeof(libs), "-l");
            NDRX_STRCAT_S(libs, sizeof(libs), iter->qname);
        }
        iter = iter->next;
    }
    
    include_path[0]=EXEOS;
    iter = ndrx_G_incpath;
    
    first = EXTRUE;
    while (iter)
    {
        
        /* if lib path is relative, then add original WD */
        
        if ('/'!=iter->qname[0])
        {
            snprintf(tmp, sizeof(tmp), "%s/%s", ndrx_G_owd, iter->qname);
        }
        else
        {
            NDRX_STRCPY_SAFE(tmp, iter->qname);
        }
        
        if (first)
        {
            NDRX_STRCAT_S(include_path, sizeof(include_path), "-I");
            first=EXFALSE;
        }
        else
        {
            NDRX_STRCAT_S(include_path, sizeof(include_path), " ");
            NDRX_STRCAT_S(include_path, sizeof(include_path), "-I");
        }
        
        NDRX_STRCAT_S(include_path, sizeof(include_path), tmp);
        
        iter = iter->next;
    }
    
    libpath_path[0]=EXEOS;
    iter = ndrx_G_libpath;
    
    first = EXTRUE;
    while (iter)
    {
        /* if lib path is relative, then add original WD */
        
        if ('/'!=iter->qname[0])
        {
            snprintf(tmp, sizeof(tmp), "%s/%s", ndrx_G_owd, iter->qname);
        }
        else
        {
            NDRX_STRCPY_SAFE(tmp, iter->qname);
        }
        
        if (first)
        {
            NDRX_STRCAT_S(libpath_path, sizeof(libpath_path), "-L");
            first=EXFALSE;
        }
        else
        {
            NDRX_STRCAT_S(libpath_path, sizeof(libpath_path), " ");
            NDRX_STRCAT_S(libpath_path, sizeof(libpath_path), "-L");
        }
        
        NDRX_STRCAT_S(libpath_path, sizeof(libpath_path), tmp);
        
        iter = iter->next;
    }
    
    if (EXEOS!=include_path[0] || EXEOS!=libpath_path[0])
    {
        snprintf(buildcmd, sizeof(buildcmd), "%s -n -o '%s' -f '%s %s' -a \"%s %s\"", 
                ndrx_G_build_cmd, ndrx_G_out_bin, FILENAME, libs,
                include_path, libpath_path);
    }
    else
    {
        snprintf(buildcmd, sizeof(buildcmd), "%s -n -o '%s' -f '%s %s'", 
                ndrx_G_build_cmd, ndrx_G_out_bin, FILENAME, libs);
    }
    
    NDRX_LOG(log_debug, "%s", buildcmd);
    fprintf(stderr, "%s\n", buildcmd);
    ret = system(buildcmd);
    
    if (EXSUCCEED!=ret)
    {
        NDRX_LOG(log_debug, "%s failed %d", buildcmd, ret);
        EXFAIL_OUT(ret);
    }
    
    if (test_mode)
    {
        NDRX_LOG(log_debug, "running int test mode -> try to startup "
                "binary (check classes)...");
        
        snprintf(buildcmd, sizeof(buildcmd), "./%s", ndrx_G_out_bin);
        
        
        NDRX_LOG(log_debug, "%s", buildcmd);
        fprintf(stderr, "%s\n", buildcmd);
        ret = system(buildcmd);

        if (EXSUCCEED!=ret)
        {
            NDRX_LOG(log_debug, "%s failed %d", buildcmd, ret);
            EXFAIL_OUT(ret);
        }
    }
    
out:
    
    if (NULL!=f)
    {
        NDRX_FCLOSE(f);
    }
    
    return ret;
    
}

/* vim: set ts=4 sw=4 et cindent: */
