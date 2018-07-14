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

#define  _POSIX_C_SOURCE 200809L

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdlib.h>
#include <ndrx_config.h>
#include <atmi.h>
#include <sys_unix.h>
#include <ctype.h>

#include <ubf.h>
#include <ndrstandard.h>
#include <ndebug.h>
#include <view_cmn.h>

#include "exjld.h"

/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
#define TEMPDIR_MASK    "exjld.XXXXXX"
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
expublic char ndrx_G_build_cmd[PATH_MAX+1] = "buildserver";
expublic int ndrx_G_do_test = EXTRUE;   /** shall we run testing? */
expublic char ndrx_G_main_class[PATH_MAX+1] = "";
expublic char ndrx_G_out_bin[PATH_MAX+1] = "a.out";

/** work directory where all temp files will go */
expublic char ndrx_G_wd[PATH_MAX+1] = "";

/** original work directory at startup */
expublic char ndrx_G_owd[PATH_MAX+1] = "";


/**
 * Runtime and build library path
 */
expublic string_list_t* ndrx_G_libpath = NULL;

/**
 * Override libs for build
 */
expublic string_list_t* ndrx_G_libs = NULL;

/**
 * Resource files
 */
expublic string_list_t* ndrx_G_embedded_res = NULL;

/** shall we keep temp files? */
expublic int ndrx_G_keep_temp = EXFALSE;

/** List of classes used for compilation */
expublic ndrx_growlist_t ndrx_G_classes;

/** List of resources to be embedded */
expublic ndrx_growlist_t ndrx_G_resources;

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
    fprintf (stderr, 
    "Mandatory arguments.\n"
    "   -m 'class_name'     Main class name\n\n"
    "Optional arguments.\n"
    "   -o 'binary_name'    Output binary name. Default is 'a.out'\n"
    "   -l 'libs'           Override Java libraries included in link (default \n"
    "                        -ljava -ljvm) \n"
    "   -L 'library_path'   Library search path for Enduro/X, System and Java\n"
    "   -b 'build_cmd'      Build command, default is Enduro/X 'buildserver'\n"
    "   -n                  Do not perform test run (i.e. check class loader)\n"
    "   -t 'temp_dir_pfx'   Tempdir prefix instead of './'.\n"
    "   -e 'file_name'      Embed resource into exe file. Multiple occurrences\n"
    "                        are allowed. File name must be unique among embedded.\n"
    "                        Accessed via org.endurox.AtmiCtx.getResource(<name>);\n"
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
    int c;
    int was_file = EXFALSE;
    char *env;
    char tmp[PATH_MAX];
    int i;
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
    while ((c = getopt (argc, argv, "m:o:L:l:b:nt:ke:")) != -1)
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
                ndrx_G_do_test = EXFALSE;        
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
                
                NDRX_LOG(log_debug, "Adding library: [%s] (override)", optarg);
                
                if (EXSUCCEED!=ndrx_string_list_add(&ndrx_G_libs, optarg))
                {
                    NDRX_LOG(log_error, "Failed to add: [%s] to ndrx_G_libs",
                            optarg);
                    EXFAIL_OUT(ret);
                }

                break;
            case 'e':
                
                NDRX_LOG(log_debug, "Embedding resource: [%s]", optarg);
                if (EXSUCCEED!=ndrx_string_list_add(&ndrx_G_embedded_res, optarg))
                {
                    NDRX_LOG(log_error, "Failed to add: [%s] to ndrx_G_embedded_res",
                            optarg);
                    EXFAIL_OUT(ret);
                }
                break;
            case 'k':
                ndrx_G_keep_temp = EXTRUE;
                break;
            case 't':
                snprintf(ndrx_G_wd, sizeof(ndrx_G_wd), "%s/%s", optarg, TEMPDIR_MASK);
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
    
    /* if libs not set, then use defaults */
    if (NULL==ndrx_G_libs)
    {
        char *libs[] = {"java", "jvm"};
        
        for (i=0; i<N_DIM(libs); i++)
        {
            NDRX_LOG(log_debug, "Adding library: [%s]", libs[i]);

            if (EXSUCCEED!=ndrx_string_list_add(&ndrx_G_libs, libs[i]))
            {
                NDRX_LOG(log_error, "Failed to add: [%s] to ndrx_G_libs",
                        optarg);
                EXFAIL_OUT(ret);
            }
        }
    }
    
    /* create directory */
    
    if (EXEOS==ndrx_G_wd[0])
    {
        snprintf(ndrx_G_wd, sizeof(ndrx_G_wd), "./%s", TEMPDIR_MASK);
    }
    
    NDRX_LOG(log_debug, "Temp dir template set to: [%s]", ndrx_G_wd);
    
    if (NULL==mkdtemp (ndrx_G_wd))
    {
        NDRX_LOG(log_error, "Failed to create working directory by template [%s]: %s",
                ndrx_G_wd, strerror(errno));
        EXFAIL_OUT(ret);
    }    
    
    /* save current folder.. */
    if (NULL==getcwd(ndrx_G_owd, sizeof(ndrx_G_owd)))
    {
        ndrx_G_owd[0] = EXEOS;
        NDRX_LOG(log_error, "Failed to get working directory: %s", 
                strerror(errno));
        EXFAIL_OUT(ret);
    }
    
    /* Change directory */
    if (EXFAIL==chdir (ndrx_G_wd))
    {
        NDRX_LOG(log_error, "Failed to chdir to [%s]: %s", 
                ndrx_G_wd, strerror(errno));
        EXFAIL_OUT(ret);
    }

    for (i = optind; i < argc; i++)
    {
        int sysret;
        was_file = EXTRUE;
        NDRX_LOG (log_debug, "Processing JAR file [%s]", argv[i]);
        
        /* extract file... */
        snprintf(tmp, sizeof(tmp), "jar xf %s/%s", ndrx_G_owd, argv[i]);
        
        NDRX_LOG(log_debug, "%s", tmp);
        
        sysret = system(tmp);
        
        if (EXSUCCEED!=sysret)
        {
            NDRX_LOG(log_error, "Failed to execute: [%s]: %d", tmp, sysret);
            EXFAIL_OUT(ret);
        }
    }
    
    
    /* TODO: build list of files, with structures, allocate linear array */
    
    /* TODO: generate resource files with exjlib_N+.cinclude */
    
    /* TODO: allocate linear array for embedded resources */
    
    /* TODO: generate embedded resources with exjemb_N.cinclude */
    
    /* TODO: generate C build file (test mode on) */
    
    /* TODO: build */
    
    /* TODO: run test */
    
    /* TODO: generate C build file (test mode off) */
    
    /* TOOD: build */
    
    /* ... and we are done! */
    
    if (!was_file)
    {
        usage(argv[0]);
        EXFAIL_OUT(ret);
    }
    
out:
                
    if (EXEOS!=ndrx_G_owd[0])
    {
        /* change dir back */
        if (EXSUCCEED!=chdir(ndrx_G_owd))
        {
            NDRX_LOG(log_warn, "Failed to chdir back to [%s]: %s", 
                    ndrx_G_owd, strerror(errno));
        }
    }
    /* So we need to load the view file now and generate header */
    return ret;
}

/* vim: set ts=4 sw=4 et cindent: */