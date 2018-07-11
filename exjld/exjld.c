/**
 * @brief Enduro/X Java Linker, main entry point generates extracts jar files
 *  Generates include resources from the class files, build the array, and sorts
 *  the class data. Then generates C file which is compiled with buildserver(8)
 *  command with custom main entry. The main class shall be specified at build time.
 *  Also include/library path to libraries shall be given too (i.e. where to find
 *  java VM libs and headers)
 *
 * @file exjld.c
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

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <ndrx_config.h>
#include <atmi.h>
#include <atmi_int.h>
#include <sys_unix.h>
#include <ctype.h>

#include <ubf.h>
#include <ferror.h>
#include <fieldtable.h>
#include <fdatatype.h>

#include <ndrstandard.h>
#include <ndebug.h>
#include <view_cmn.h>
#include "viewc.h"

/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
expublic char ndrx_G_build_cmd[PATH_MAX+1] = "buildserver";
expublic int ndrx_G_do_test = EXTRUE;   /** shall we run testing? */
expublic char ndrx_G_main_class[PATH_MAX+1] = "";
expublic char ndrx_G_out_bin[PATH_MAX+1] = "a.out";

/**
 * Runtime and build library path
 */
expublic string_list_t* ndrx_G_libpath = NULL;

/**
 * Override libs for build
 */
expublic string_list_t* ndrx_G_libs = NULL;


/** shall we keep temp files? */
expublic int ndrx_G_keep_temp = EXFALSE;



/*---------------------------Statics------------------------------------*/

/**
 * print usage
 * @param argc
 * @param argv
 * @return 
 */
exprivate void usage(char *progname)
{
    fprintf (stderr, "Usage: %s [OPTION]... JAR...\n\n", progname);
    fprintf (stdrr, 
    "Mandatory arguments.\n"
    "   -m 'class_name'     Main class name\n\n"
    "Optional arguments.\n"
    "   -o 'binary_name'    Output binary name. Default is 'a.out'\n"
    "   -l 'libs'           Override Java libraries included in link (default \n"
    "                        -ljava -ljvm) \n"
    "   -L 'library_path'   Library search path for Enduro/X, System and Java\n"
    "   -b 'build_cmd'      Build command, default is Enduro/X 'buildserver'\n"
    "   -n                  Do not perform test run (i.e. check class loader)\n"
    "   -t 'temp_dir'       Override temporary folder instead of ./<generated>.\n"
    "                        If override is made, temp folder is not removed\n"
    "   -k                  Keep temp files/folder when running in non -t mode\n"
    "Example.\n"
    "   $ exjld -o testbin -L /usr/lib/jvm/java-8-openjdk-amd64/jre/lib/amd64 \\"
    "      test1.jar hamcrest-core-1.3.jar junit-4.12.jar\n");
}

/**
 * Main entry point for view compiler
 */
int main(int argc, char **argv)
{
    int ret = EXSUCCEED;
    opterr = 0;
    
    fprintf(stderr, "Enduro/X Java Linker\n\n");

    /*
    TODO: Parse config, create temp directory, cd temp
    extract jars(with ../ in front) to current dir
    search for all class files, generate resources from them with ndrx_N+ name
    in current directory. Firstly we need an array of all of thee resources, which
    later will be compiled and linked into the output binary.
    the output binary too shall go to ../. Also we need to save somewhere binary name
    so that we could quickly provide it back to user App if requested by Enduro/X APIs
    */
    /*
     * Options:
     * m: - main class name
     * o: - output binary name
     * I: - include header search path
     * L: - include library search path
     * n - do not run test
     * 
     */
    while ((c = getopt (argc, argv, "m:o:L:l:b:nt:k")) != -1)
    {
        switch (c)
        {
            case 'L':
                /* Extra Library for runtime, colon separated...
                 * this shall also be used for build time
                 */
                env = getenv(NDRX_LD_LIBRARY_PATH);
                
                if (NULL!=env)
                {
                    snprintf(tmp, sizeof(tmp), "%s:%s", env, optarg);
                }
                else
                {
                    NDRX_STRCPY_SAFE(tmp, optarg);
                }
                
                NDRX_LOG(log_debug, "Setting library path: [%s]", tmp);
                
                setenv(NDRX_LD_LIBRARY_PATH, tmp, EXTRUE);
                        
                if (EXSUCCEED!=ndrx_string_list_add(&ndrx_G_libpath, optarg))
                {
                    NDRX_LOG(log_error, "Failed to add: [%s] to ndrx_G_libpath",
                            optarg);
                    EXFAIL_OUT(ret);
                }
                
                break;
            case 'b':
                NDRX_STRCPY_SAFE(ndrx_G_build_cmd, optarg);
                break;
            case 'n':
                NDRX_LOG(log_debug, "Testing not required");
                M_do_test = EXFALSE;        
                break;
            case 'm':
                NDRX_STRCPY_SAFE(ndrx_G_main_class, optarg);
                NDRX_LOG(log_debug, "Main class set to: [%s]", 
                        ndrx_G_main_class);
                break;
            case 'o':
                NDRX_STRCPY_SAFE(ndrx_G_out_bin, optarg);
                NDRX_LOG(log_debug, "Out binary set to: [%s]", 
                        ndrx_G_out_bin);
                break;
            case 'l':
                if (EXSUCCEED!=ndrx_string_list_add(&ndrx_G_libs, optarg))
                {
                    NDRX_LOG(log_error, "Failed to add: [%s] to ndrx_G_libs",
                            optarg);
                    EXFAIL_OUT(ret);
                }

                break;
            case 'h':
                usage(argv[0]);
                EXFAIL_OUT(ret);
                break;
            case '?':
                if (optopt == 'c')
                {
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                }
                else if (isprint (optopt))
                {
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                }
                else
                {
                    fprintf (stderr,
                        "Unknown option character `\\x%x'.\n",
                        optopt);
                }
                EXFAIL_OUT(ret);
            default:
                NDRX_LOG(log_error, "Default case...");
                EXFAIL_OUT(ret);
        }
    }
    
    NDRX_LOG(log_debug, "Build command set to: [%s]", ndrx_G_build_cmd);
    for (i = optind; i < argc; i++)
    {
        was_file = EXTRUE;
        NDRX_LOG (log_debug, "Processing VIEW file [%s]", argv[i]);
        
        /* Load the view file */
        if (EXSUCCEED!=ndrx_view_load_file(argv[i], EXFALSE))
        {
            NDRX_LOG(log_error, "Failed to load view file [%s]: %s", 
                    argv[i], Bstrerror(Berror));
            EXFAIL_OUT(ret);
        }
        
        /* get the base name of output file */
        
        NDRX_STRCPY_SAFE(basename, argv[i]);
        p = strrchr(basename, '.');
        
        if (NULL!=p)
        {
            *p=EXEOS;
        }
        
        if (strlen(basename) == 0)
        {
            NDRX_LOG(log_error, "Invalid view file name passed on CLI...");
            EXFAIL_OUT(ret);
        }
        
        /* Plot the the header */
        if (HDR_C_LANG==lang_mode)
        {
            if (EXSUCCEED!=ndrx_view_plot_c_header(outdir, basename))
            {
                NDRX_LOG(log_error, "Failed to plot c header!");
                EXFAIL_OUT(ret);
            }
        
            /* Get the offset - generate object file & invoke */
            
            if (EXSUCCEED!=ndrx_view_generate_code(outdir, basename, argv[i], 
                    Vfile, no_UBF))
            {
                NDRX_LOG(log_error, "Failed to generate code or invoke compiler!");
                EXFAIL_OUT(ret);
            }
        }
        
        /* Unload the view files (remove from hashes) */
        ndrx_view_deleteall();
        
        NDRX_LOG(log_info, ">>> About to test object file...");
        
        if (EXSUCCEED!=ndrx_view_load_file(Vfile, EXTRUE))
        {
            NDRX_LOG(log_error, "!!! Failed to test object file [%s] !!!", Vfile);
            EXFAIL_OUT(ret);
        }
        
        ndrx_view_deleteall();
        
        
        NDRX_LOG(log_info, ">>> [%s] COMPILED & TESTED OK!", Vfile);
        
    }
    
    if (!was_file)
    {
        usage(argv[0]);
        EXFAIL_OUT(ret);
    }
    
out:
    /* So we need to load the view file now and generate header */
    return ret;
}


