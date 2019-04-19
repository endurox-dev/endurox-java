/**
 * @brief Parse XA Configuration
 *
 * @file xaconfig.c
 */
/* -----------------------------------------------------------------------------
 * Enduro/X Middleware Platform for Distributed Transaction Processing
 * Copyright (C) 2009-2016, ATR Baltic, Ltd. All Rights Reserved.
 * Copyright (C) 2017-2018, Mavimax, Ltd. All Rights Reserved.
 * This software is released under one of the following licenses:
 * AGPL or Mavimax's license for commercial use.
 * -----------------------------------------------------------------------------
 * AGPL license:
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License, version 3 as published
 * by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU Affero General Public License, version 3
 * for more details.
 *
 * You should have received a copy of the GNU Affero General Public License along 
 * with this program; if not, write to the Free Software Foundation, Inc., 
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * -----------------------------------------------------------------------------
 * A commercial use license is available from Mavimax, Ltd
 * contact@mavimax.com
 * -----------------------------------------------------------------------------
 */
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <ctype.h>
#include <errno.h>

#include <userlog.h>

#include <ndrstandard.h>
#include <ndebug.h>

#include <exparson.h>
#include <ubf.h>

/*------------------------------Externs---------------------------------------*/
/*------------------------------Macros----------------------------------------*/
#define NDRXJ_FS            0x1C
/*------------------------------Enums-----------------------------------------*/
/*------------------------------Typedefs--------------------------------------*/
/*------------------------------Globals---------------------------------------*/
/*------------------------------Statics---------------------------------------*/
/*------------------------------Prototypes------------------------------------*/

/**
 * Parse XA Config.
 * JSON to parse is following:
 * 
 * {"class":"org.postgresql.xa.PGXADataSource", "props":{"PROP":"VAL", "PROP2":"VAL2"}, 
 *  "set":{"SetHost":"192.168.0.1", "SetPort":"7777"}}
 * 
 * Output is following:
 * class = "org.postgresql.xa.PGXADataSource"
 * set = ["SetHost<FS>192.168.0.1","SetPort<FS>7777"]
 * props = ["PROP<FS>VAL", "PROP2<FS>VAL2"]
 * 
 * @param [in] buffer - json text to parse
 * @param [out] props - parsed properties of XA string. The key/value separator
 *  is "\n" - newline.
 * @param [out] nrprops number of properties loaded
 * @param [out[ sets - parsed set commands. The key/value separator
 *  is "\n" - newline.
 * @param [out] nrsets number of strings loaded in sets
 * @param [out] clazz driver class
 * @param [out] clazz_bufsz buffer size of class
 * @return SUCCEED/FAIL
 */
expublic int ndrxj_xa_cfgparse(char *buffer, string_list_t **props, int *nrprops,
            string_list_t **sets, int *nrsets, char *clazz, int clazz_bufsz)
{
    int ret = EXSUCCEED;
    EXJSON_Value *root_value=NULL;
    EXJSON_Object *root_object;
    EXJSON_Object *sub_obj;    
    size_t i, cnt, j, sub_cnt;
    char *name, *sub_name;
    char    *str_val;
    char tmpbuf[2048];
    
    NDRX_LOG(log_debug, "Parsing buffer: [%s]", buffer);

    root_value = exjson_parse_string_with_comments(buffer);

    if (exjson_value_get_type(root_value) != EXJSONObject)
    {
        NDRX_LOG(log_debug, "Failed to parse root element");
        EXFAIL_OUT(ret);
        goto out;
    }
    root_object = exjson_value_get_object(root_value);

    cnt = exjson_object_get_count(root_object);
    NDRX_LOG(log_debug, "cnt = %d", cnt);
    
    for (i=0; i<cnt; i++)
    {
        name = (char *)exjson_object_get_name(root_object, i);
        
        if (0==strcmp(name, "class"))
        {
            sub_obj = exjson_object_get_object(root_object, name);
            sub_cnt = exjson_object_get_count(sub_obj);
                    
            for (j=0; j<sub_cnt; j++)
            {
                sub_name = (char *)exjson_object_get_name(sub_obj, i);
                
                if (0==strcmp(sub_name, "class"))
                {
                    str_val = (char *)exjson_object_get_string(sub_obj, sub_name);
                    NDRX_STRNCPY_SAFE(clazz, str_val, clazz_bufsz);
                    NDRX_LOG(log_debug, "Got JDBC XA Class: [%s]", clazz);
                }
            }
        }
        else if (0==strcmp(name, "props") || 
                0==strcmp(name, "set"))
        {
            sub_obj = exjson_object_get_object(root_object, name);
            sub_cnt = exjson_object_get_count(sub_obj);
                    
            for (j=0; j<sub_cnt; j++)
            {
                sub_name = (char *)exjson_object_get_name(sub_obj, i);
                str_val = (char *)exjson_object_get_string(sub_obj, sub_name);
                
                /* Add to queue the argument - data seperator is newline ..*/
                snprintf(tmpbuf, sizeof(tmpbuf), "%s%c%s", sub_name, NDRXJ_FS, str_val);
                
                NDRX_LOG(log_debug, "Got property: [%s]", tmpbuf);
                
                if (0==strcmp(name, "props"))
                {
                    if (EXSUCCEED!=ndrx_string_list_add(props, tmpbuf))
                    {
                        NDRX_LOG(log_error, "Failed to add props to list: [%s]", 
                                tmpbuf);
                        EXFAIL_OUT(ret);
                    }
                    (*nrprops)++;
                }
                else
                {
                    if (EXSUCCEED!=ndrx_string_list_add(sets, tmpbuf))
                    {
                        NDRX_LOG(log_error, "Failed to add props to list: [%s]", 
                                tmpbuf);
                        EXFAIL_OUT(ret);
                    }
                    (*nrsets)++;
                }
            }
        } /* for settings */ 
        else
        {
            NDRX_LOG(log_warn, "Skipping [%s] - unsupported", name);
        }
    } /* for root object elements */
    
out:
    /* cleanup code */
    if (NULL != root_value)
    {
        exjson_value_free(root_value);
    }
    
    return ret;
}

/* vim: set ts=4 sw=4 et smartindent: */
