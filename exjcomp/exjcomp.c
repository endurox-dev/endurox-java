/* 
 * @brief Compile stand alone java binary
 *
 * @file exjcomp.c
 */

/*
 * -----------------------------------------------------------------------------
 * Enduro/X Middleware Platform for Distributed Transaction Processing
 * Copyright (C) 2015, Mavimax, Ltd. All Rights Reserved.
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

#include <ndrstandard.h>
#include <ndebug.h>

/*---------------------------Externs------------------------------------*/
/*---------------------------Macros-------------------------------------*/
/*---------------------------Enums--------------------------------------*/
/*---------------------------Typedefs-----------------------------------*/
/*---------------------------Globals------------------------------------*/
expublic char ndrx_G_build_cmd[PATH_MAX+1] = "buildclient";
/*---------------------------Statics------------------------------------*/

/**
 * print usage
 * @param argc
 * @param argv
 * @return 
 */
exprivate void usage(char *progname)
{
    fprintf (stderr, "Usage: %s [-o outfile] [-b build_command] "
                "[-L extra_c_lib] jarfile [jarfile ... ]\n", 
                progname);
}

/**
 * Main entry point for view compiler
 */
int main(int argc, char **argv)
{
    int no_UBF = EXFALSE;
    char outfile[PATH_MAX+1]="a.out";
    int i;
    int c;
    int ret = EXSUCCEED;
    int lang_mode = HDR_C_LANG;
    char basename[PATH_MAX+1];
    char *p, *env;
    int was_file = EXFALSE;
    char tmp[PATH_MAX];
    opterr = 0;
    
    NDRX_BANNER;
    
    fprintf(stderr, "Enduro/X Java Compiler\n\n");

    while ((c = getopt (argc, argv, "ho:L:")) != -1)
    {
        switch (c)
        {
            case 'L':
                /* Extra Library for runtime, colon separated...*/
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
                
                break;
            case 'b':
                NDRX_STRCPY_SAFE(ndrx_G_build_cmd, optarg);
                break;
            case 'd':
                NDRX_STRCPY_SAFE(outdir, optarg);
                NDRX_LOG(log_debug, "Changing view object output directory to: [%s]", 
                        outdir);
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
        NDRX_LOG (log_debug, "Processing JAR file [%s]", argv[i]);
        
        /* TODO: Generate header (generate temp file name) */
    }
    
    if (!was_file)
    {
        usage(argv[0]);
        EXFAIL_OUT(ret);
    }

    /* Generate C program to run off the JVM... */

    /* Compile the program to get output binary... */
    
out:
    /* So we need to load the view file now and generate header */
    return ret;
}


