/**
 * @brief Enduro/X Java Linker, main entry point generates extracts jar files
 *   Generates include resources from the class files, build the array, and sorts
 *   the class data. Then generates C file which is compiled with buildserver(8)
 *   command with custom main entry. The main class shall be specified at build time.
 *   Also include/library path to libraries shall be given too (i.e. where to find
 *   java VM libs and headers)
 *   All files from jars ar included in class lists, with file extensions.
 *   resources like properties are generated and stored in hash as file path
 *   classes are stored with dot syntax.
 *   We could store the resources in resource list for performance and
 *   name collision reasons.
 *
 * @file exjld.c
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

#define  _POSIX_C_SOURCE 200809L

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdlib.h>
#include <ndrx_config.h>
#include <atmi.h>
/* #include <sys_unix.h> */
#include <ctype.h>

#include <ubf.h>
#include <ndrstandard.h>
#include <ndebug.h>
#include <view_cmn.h>
#include <ndrx_java_config.h>
#include <exthpool.h>
#include <thlock.h>
#include <sys_unix.h>
#include "exjld.h"
#include "zip.h"

/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
#define TEMPDIR_MASK    "exjld.XXXXXX"
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
expublic char ndrx_G_build_cmd[PATH_MAX+1] = "buildserver";
expublic int ndrx_G_do_test = EXTRUE;   /** shall we run testing? */
expublic char ndrx_G_main_class[PATH_MAX+1] = "";
expublic char ndrx_G_out_bin[PATH_MAX+1] = "";

/** work directory where all temp files will go */
expublic char ndrx_G_wd[PATH_MAX+1] = "";

/** original work directory at startup */
expublic char ndrx_G_owd[PATH_MAX+1] = "";

/** Runtime and build library path */
expublic string_list_t* ndrx_G_libpath = NULL;

/** Include pathes */
expublic string_list_t* ndrx_G_incpath = NULL;

/** Override libs for build */
expublic string_list_t* ndrx_G_libs = NULL;

/** shall we keep temp files? */
expublic int ndrx_G_keep_temp = EXFALSE;

/** List of classes used for compilation */
expublic exjld_resource_t *ndrx_G_classes_hash = NULL;

/** List of resources to be embedded */
expublic exjld_resource_t *ndrx_G_emb_res_hash = NULL;

/** Linker utility thread pool */
expublic threadpool ndrx_G_thpool = NULL;

/** 
 * Thread pool global error response
 * Also we need mutex lock for this
 */
expublic int ndrx_G_thpool_error = EXSUCCEED;
MUTEX_LOCKDECL(ndrx_G_thpool_error_lock);
MUTEX_LOCKDECL(ndrx_G_thpool_dbg_lock);

/*---------------------------Statics------------------------------------*/

/**
 * Set thread error
 * @param ret error code
 */
expublic void exjld_thread_error_set(int ret)
{
    MUTEX_LOCK_V(ndrx_G_thpool_error_lock);
    
    ndrx_G_thpool_error = ret;
    
    MUTEX_UNLOCK_V(ndrx_G_thpool_error_lock);
}

/**
 * Perform debug locking for clean console output (not overlapping)
 */
expublic void exjld_thread_debug_lock(void)
{
    MUTEX_LOCK_V(ndrx_G_thpool_dbg_lock);
}

/**
 * Unlock the debug output
 */
expublic void exjld_thread_debug_unlock(void)
{
    MUTEX_UNLOCK_V(ndrx_G_thpool_dbg_lock);
}

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
    "                        -ljava -ljvm -lexjlds) \n"
    "   -L 'library_path'   Library search path for Enduro/X, System and Java\n"
    "   -I 'include_path'   Include path for header (Enduro/X Java module)\n"
    "   -b 'build_cmd'      Build command, default is Enduro/X 'buildserver'\n"
    "   -n                  Do not perform test run (i.e. check class loader)\n"
    "   -t 'temp_dir_pfx'   Tempdir prefix instead of './'.\n"
    "                        Accessed via org.endurox.AtmiCtx.getResource(<name>);\n"
    "   -k                  Keep temp files/folder when running in non -t mode\n"
    "   -j 'nr_jobs'        Number of concurrent jobs, default is 4\n"
    "Example.\n"
    "   $ exjld -o testbin -L /usr/lib/jvm/java-8-openjdk-amd64/jre/lib/amd64 \\\n"
    "      test1.jar hamcrest-core-1.3.jar junit-4.12.jar\n");
}

/**
 * extract jar files, threaded
 * @param ptr ptr to file name
 * @param p_finish_off shall we finish, not touching...
 */
void extract_jar_th (void *ptr, int *p_finish_off)
{
    int ret = EXSUCCEED;
    char tmp[PATH_MAX];
    int sysret;
    char *path = (char *)ptr;
    
    exjld_thread_debug_lock();
    NDRX_LOG (log_debug, "Processing JAR file [%s]", path);
    exjld_thread_debug_unlock();

    /* extract file... */
    if ('/'==path[0])
    {
        /*
        snprintf(tmp, sizeof(tmp), "jar xf %s", path);
         * */
        
        snprintf(tmp, sizeof(tmp), "%s", path);
    }
    else
    {
        /*
        snprintf(tmp, sizeof(tmp), "jar xf %s/%s", ndrx_G_owd, path);
        */
        snprintf(tmp, sizeof(tmp), "%s/%s", ndrx_G_owd, path);
    }

    exjld_thread_debug_lock();
    NDRX_LOG(log_debug, "%s", tmp);
    fprintf(stderr, "%s\n", tmp);
    exjld_thread_debug_unlock();

/*
    ret = system(tmp);
 * 
 * if (EXSUCCEED!=ret)
    {
        exjld_thread_debug_lock();
        NDRX_LOG(log_error, "Failed to execute: [%s]: %d", tmp, sysret);
        exjld_thread_debug_unlock();
        goto out;
    }
 * 
*/    
    ret = zip_extract(tmp, ".", NULL, NULL);
    
    if (EXFAIL==ret)
    {
        exjld_thread_debug_lock();
        NDRX_LOG(log_error, "Failed to open zip file: [%s] missing file or invalid format!",
                tmp);
        exjld_thread_debug_unlock();
        goto out;
    }
    
out:
    
    if (EXSUCCEED!=ret)
    {
        exjld_thread_error_set(ret);
    }
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
    char tmp_outbin[PATH_MAX]="a.out";
    int i;
    int tmpi;
    int jobs = 4;
    opterr = 0;
    
    /* Print Enduro/X Banner */
    ndrx_sys_banner();
    
    fprintf(stderr, "\n%s, Linker\n\n", NDRX_JAVA_VERSION);

    /*
    Parse config, create temp directory, cd temp
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
    while ((c = getopt (argc, argv, "m:o:L:l:b:nt:ke:I:hj:")) != -1)
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
            case 'I':
                NDRX_LOG(log_debug, "Adding [%s] to include path", optarg);
                
                if (EXSUCCEED!=ndrx_string_list_add(&ndrx_G_incpath, optarg))
                {
                    NDRX_LOG(log_error, "Failed to add: [%s] to ndrx_G_incpath",
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
                NDRX_STRCPY_SAFE(tmp_outbin, optarg);
                NDRX_LOG(log_debug, "Out binary set to: [%s]", 
                        tmp_outbin);
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
            case 'j':
                
                tmpi = atoi(optarg);
                
                if (tmpi > 0)
                {
                    jobs = tmpi;
                }
                else
                {
                    NDRX_LOG(log_error, "Invalid setting [%s] for 'j' arg - "
                            "cannot be less than 0",
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
    
    if (EXEOS==ndrx_G_main_class[0])
    {
        NDRX_LOG(log_error, "Main class name (-m) must be specified!");
        usage(argv[0]);
        EXFAIL_OUT(ret);
    }
    
    NDRX_LOG(log_debug, "Build command set to: [%s]", ndrx_G_build_cmd);
    
    /* if libs not set, then use defaults */
    if (NULL==ndrx_G_libs)
    {
        char *libs[] = {"exjlds", "java", "jvm"};
        
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
    
    if (NULL==(ndrx_G_thpool = thpool_init(jobs)))
    {
        NDRX_LOG(log_error, "Failed to initialize thread pool (cnt: %d)!", 
                jobs);
        EXFAIL_OUT(ret);
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
    
    /* compute output binary name.. */
    
    if (EXEOS==tmp_outbin[0])
    {
        /* it is full path */
        NDRX_STRCPY_SAFE(ndrx_G_out_bin, tmp_outbin);
    }
    else
    {
        /* it is relative to  */
        snprintf(ndrx_G_out_bin, sizeof(ndrx_G_out_bin), "%s/%s",
                ndrx_G_owd, tmp_outbin);
    }
    
    NDRX_LOG(log_debug, "Final output binary: [%s]", ndrx_G_out_bin);
    
    /* Change directory */
    if (EXFAIL==chdir (ndrx_G_wd))
    {
        NDRX_LOG(log_error, "Failed to chdir to [%s]: %s", 
                ndrx_G_wd, strerror(errno));
        EXFAIL_OUT(ret);
    }
    
    /* run extract in parallel */
    exjld_thread_error_set(EXSUCCEED);
    
    for (i = optind; i < argc; i++)
    {
        was_file = EXTRUE;
        thpool_add_work(ndrx_G_thpool, (void*)extract_jar_th, (void *)argv[i]);
    }
    
    if (was_file)
    {
        thpool_wait(ndrx_G_thpool);
        
        if (EXSUCCEED!=ndrx_G_thpool_error)
        {
            NDRX_LOG(log_error, "Failed to extract JARs!");
            EXFAIL_OUT(ret);
        }
    }
    
    /* generate resource files with exjlib_N+.cinclude */
    if (EXSUCCEED!=exjld_class_build_hash())
    {
        NDRX_LOG(log_error, "Failed to generate classes!");
        EXFAIL_OUT(ret);
    }
    
    if (!was_file)
    {
        NDRX_LOG(log_error, "No JAR file was specified!");
        usage(argv[0]);
        EXFAIL_OUT(ret);
    }
    
    /* Sort hashes */
    
    exljd_res_sort_by_resname(&ndrx_G_classes_hash);
    exljd_res_sort_by_resname(&ndrx_G_emb_res_hash);
    
    /* generate C build file (test mode on) */
    
    if (ndrx_G_do_test)
    {
        if (EXSUCCEED!=ndrxj_codegen(EXTRUE))
        {
            NDRX_LOG(log_debug, "Failed to build test binary!");
            EXFAIL_OUT(ret);
        }
    }
    
    /* final build */
    if (EXSUCCEED!=ndrxj_codegen(EXFALSE))
    {
        NDRX_LOG(log_debug, "Failed to build a binary!");
        EXFAIL_OUT(ret);
    }
    
    NDRX_LOG(log_info, "Build ok");
    
out:
                
    if (NULL!=ndrx_G_thpool)
    {
        thpool_wait(ndrx_G_thpool);
        thpool_destroy(ndrx_G_thpool);
    }
                
    if (EXEOS!=ndrx_G_owd[0])
    {
        /* change dir back */
        if (EXSUCCEED!=chdir(ndrx_G_owd))
        {
            NDRX_LOG(log_warn, "Failed to chdir back to [%s]: %s", 
                    ndrx_G_owd, strerror(errno));
        }
    }

    /* remove work directory */

    if (!ndrx_G_keep_temp && EXEOS!=ndrx_G_wd[0])
    {
        int rm_res;
        snprintf(tmp, sizeof(tmp), "rm -rf %s", ndrx_G_wd);
        
        NDRX_LOG(log_debug, "%s", tmp);
        fprintf(stderr, "%s\n", tmp);
        rm_res = system(tmp);
        
        if (EXSUCCEED!=rm_res)
        {
            NDRX_LOG(log_error, "%s failed: %d", tmp, ret);
        }
    }

    return ret;
}

/* vim: set ts=4 sw=4 et smartindent: */
