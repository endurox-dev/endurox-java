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
#include <view2exjson.h>
#include <ubf.h>
#include <atmi_int.h>
#include <typed_buf.h>
#include <exbase64.h>

#include "tperror.h"


/*------------------------------Externs---------------------------------------*/
/*------------------------------Macros----------------------------------------*/
#define IS_INT(X) (BFLD_CHAR == X || BFLD_SHORT == X || BFLD_LONG == X || BFLD_INT == X)
#define IS_NUM(X) (BFLD_SHORT == X || BFLD_LONG == X || BFLD_FLOAT == X || BFLD_DOUBLE == X)
#define IS_BIN(X) (BFLD_CARRAY == X)

/* TODO: Fix atmi buffer size to match size of ATMI buffer size. */
#define CARR_BUFFSIZE       NDRX_MSGSIZEMAX
#define CARR_BUFFSIZE_B64   (4 * (CARR_BUFFSIZE) / 3)
/*------------------------------Enums-----------------------------------------*/
/*------------------------------Typedefs--------------------------------------*/
/*------------------------------Globals---------------------------------------*/
/*------------------------------Statics---------------------------------------*/
/*------------------------------Prototypes------------------------------------*/


exprivate long round_long( double r ) {
    return (r > 0.0) ? (r + 0.5) : (r - 0.5); 
}

/**
 * Parse XA Config.
 * @param buffer - json text to parse
 * @param [out] props - parsed properties of XA string. The key/value separator
 *  is "\n" - newline.
 * @param [out[ sets - parsed set commands. The key/value separator
 *  is "\n" - newline.
 * @return SUCCEED/FAIL
 */
expublic char* ndrxj_xa_cfgparse(char *buffer, string_list_t **props,
            string_list_t **sets)
{
    int ret = EXSUCCEED;
    EXJSON_Value *root_value=NULL;
    EXJSON_Object *root_object;
    
    EXJSON_Object *view_object;
    
    EXJSON_Array *array;
    size_t i, cnt, j, arr_cnt;
    int type;
    char *name;
    char    *str_val;
    double d_val;
    int f_type;
    short   bool_val;
    char bin_buf[CARR_BUFFSIZE+1];
    char    *s_ptr;
    long vsize;
    int cnametyp;
    char *cstruct = NULL;
    
    NDRX_LOG(log_debug, "Parsing buffer: [%s]", buffer);

    root_value = exjson_parse_string_with_comments(buffer);
    type = exjson_value_get_type(root_value);
    NDRX_LOG(log_debug, "Type is %d", type);

    if (exjson_value_get_type(root_value) != EXJSONObject)
    {
        NDRX_LOG(log_debug, "Failed to parse root element");
        ndrx_TPset_error_fmt(TPEINVAL, "exjson: Failed to parse root element");
        return NULL;
    }
    root_object = exjson_value_get_object(root_value);

    cnt = exjson_object_get_count(root_object);
    NDRX_LOG(log_debug, "cnt = %d", cnt);
    
    if (NULL==(name = (char *)exjson_object_get_name(root_object, 0)))
    {
	NDRX_LOG(log_error, "exjson: Invalid json no root VIEW object");
	ndrx_TPset_error_msg(TPEINVAL, "exjson: Invalid json no root VIEW object");
	EXFAIL_OUT(ret);
    }
    
    vsize = Bvsizeof(name);
    
    if (vsize < 0)
    {
        NDRX_LOG(log_error, "Failed to get view [%s] size: %s", 
                name, Bstrerror(Berror));
        
        ndrx_TPset_error_fmt(TPEINVAL, "Failed to get view [%s] size: %s", 
                name, Bstrerror(Berror));
        
        EXFAIL_OUT(ret);
    }
    
    NDRX_LOG(log_debug, "Allocating view [%s]: %ld", name, vsize);
    
    cstruct = tpalloc("VIEW", name, vsize);
    
    if (NULL==cstruct)
    {
        NDRX_LOG(log_error, "Failed to allocate view: %s", tpstrerror(tperrno));
        /* error must be set already! */
        EXFAIL_OUT(ret);
    }

    strcpy(view, name);
    
    view_object = exjson_object_get_object(root_object, name);
    
    if (NULL==view_object)
    {
        NDRX_LOG(log_error, "exjson: Failed to get view object");
        ndrx_TPset_error_msg(TPESYSTEM, "exjson: Failed to get view object");
        EXFAIL_OUT(ret);
    }

    cnt = exjson_object_get_count(view_object);
    NDRX_LOG(log_debug, "cnt = %d", cnt);
    
    for (i =0; i< cnt; i++)
    {
        name = (char *)exjson_object_get_name(view_object, i);

        NDRX_LOG(log_debug, "came: [%s]", name);
        
        if (EXFAIL==Bvoccur(cstruct, view, name, NULL, NULL, NULL, &cnametyp))
        {
            NDRX_LOG(log_error, "Error getting field %s.%s infos: %s",
                    view, name, Bstrerror(Berror));
            
            if (BNOCNAME==Berror)
            {
                NDRX_LOG(log_debug, "%s.%s not found in view -> ignore",
                        view, name);
                continue;
            }
            else
            {
                ndrx_TPset_error_fmt(TPESYSTEM, "Failed to get %s.%s infos: %s",
                        view, name, Bstrerror(Berror));
                EXFAIL_OUT(ret);
            }
        }

        switch ((f_type=exjson_value_get_type(exjson_object_get_value_at(view_object, i))))
        {
            case EXJSONString:
            {
                BFLDLEN str_len;
                s_ptr = str_val = (char *)exjson_object_get_string(view_object, name);
                NDRX_LOG(log_debug, "Str Value: [%s]", str_val);

                /* If it is carray - parse hex... */
                if (IS_BIN(cnametyp))
                {
                    size_t st_len;
                    NDRX_LOG(log_debug, "Field is binary..."
                            " convert from b64...");

                    if (NULL==ndrx_base64_decode(str_val,
                            strlen(str_val),
                            &st_len,
                            bin_buf))
                    {
                        NDRX_LOG(log_debug, "Failed to "
                                "decode base64!");
                        
                        ndrx_TPset_error_fmt(TPEINVAL, "Failed to "
                                "decode base64: %s", name);
                        
                        EXFAIL_OUT(ret);
                    }
                    str_len = st_len;
                    s_ptr = bin_buf;
                    NDRX_LOG(log_debug, "got binary len [%d]", str_len);
                }
                else
                {
                    str_len = strlen(s_ptr);
                }

                if (EXSUCCEED!=CBvchg(cstruct, view, name, 0, s_ptr, 
                        str_len, BFLD_CARRAY))
                {
                    NDRX_LOG(log_error, "Failed to set view field %s.%s: %s",
                            view, name, Bstrerror(Berror));
                    ndrx_TPset_error_fmt(TPESYSTEM, "Failed to set view field %s.%s: %s",
                            view, name, Bstrerror(Berror));
                    EXFAIL_OUT(ret);
                }
                
                break;
            }
            case EXJSONNumber:
            {
                long l;
                d_val = exjson_object_get_number(view_object, name);
                NDRX_LOG(log_debug, "Double Value: [%lf]", d_val);

                if (IS_INT(cnametyp))
                {
                    l = round_long(d_val);
                    if (EXSUCCEED!=CBvchg(cstruct, view, name, 0, 
                            (char *)&l, 0L, BFLD_LONG))
                    {
                        NDRX_LOG(log_error, "Failed to set [%s] to [%ld]!", 
                            name, l);
                        
                        ndrx_TPset_error_fmt(TPESYSTEM, "Failed to set [%s] to [%ld]!", 
                            name, l);
                        
                        EXFAIL_OUT(ret);
                    }
                }
                else if (EXSUCCEED!=CBvchg(cstruct, view, name, 0, 
                        (char *)&d_val, 0L, BFLD_DOUBLE))
                {
                    NDRX_LOG(log_error, "Failed to set [%s] to [%lf]: %s", 
                            name, d_val, Bstrerror(Berror));
                    
                    ndrx_TPset_error_fmt(TPESYSTEM, "Failed to set [%s] to [%lf]: %s", 
                            name, d_val, Bstrerror(Berror));
                    
                    EXFAIL_OUT(ret);
                }
            }
                    break;
            case EXJSONBoolean:
            {
                bool_val = (short)exjson_object_get_boolean(view_object, name);
                NDRX_LOG(log_debug, "Bool Value: [%hd]", bool_val);
                if (EXSUCCEED!=CBvchg(cstruct, view, name, 0, 
                        (char *)&bool_val, 0L, BFLD_SHORT))
                {
                    NDRX_LOG(log_error, "Failed to set [%s] to [%hd]: %s", 
                            name, bool_val, Bstrerror(Berror));
                    
                    ndrx_TPset_error_fmt(TPESYSTEM, "Failed to set [%s] to [%hd]: %s", 
                            name, bool_val, Bstrerror(Berror));
                    
                    EXFAIL_OUT(ret);
                }
            }
            break;
            /* Fielded buffer fields with more than one occurrance will go to array: 
             * Stuff here is almost identicial to above!
             */
            case EXJSONArray:
            {
                if (NULL==(array = exjson_object_get_array(view_object, name)))
                {
                    NDRX_LOG(log_error, "Failed to get array object!");
                    ndrx_TPset_error_fmt(TPESYSTEM, "Failed to get array object!");
                    EXFAIL_OUT(ret);
                }
                arr_cnt = exjson_array_get_count(array);

                for (j = 0; j<arr_cnt; j++ )
                {
                    switch (f_type = exjson_value_get_type(
                            exjson_array_get_value(array, j)))
                    {
                        case EXJSONString:
                        {
                            BFLDLEN str_len;
                            s_ptr = str_val = (char *)exjson_array_get_string(array, j);
                            NDRX_LOG(log_debug, 
                                        "Array j=%d, Str Value: [%s]", j, str_val);

                            /* If it is carray - parse hex... */
                            if (IS_BIN(cnametyp))
                            {
                                size_t st_len;
                                if (NULL==ndrx_base64_decode(str_val,
                                        strlen(str_val),
                                        &st_len,
                                        bin_buf))
                                {
                                    NDRX_LOG(log_debug, "Failed to "
                                            "decode base64!");
                                    
                                    ndrx_TPset_error_fmt(TPEINVAL, "Failed to "
                                            "decode base64!");
                                    
                                    EXFAIL_OUT(ret);
                                }
                                str_len = st_len;
                                s_ptr = bin_buf;
                                NDRX_LOG(log_debug, "got binary len [%d]", str_len);
                            }
                            else
                            {
                                str_len = strlen(s_ptr);
                            }

                            if (EXSUCCEED!=CBvchg(cstruct, view, name, j, 
                                    s_ptr, str_len, BFLD_CARRAY))
                            {
                                NDRX_LOG(log_error, "Failed to set [%s] to [%s]: %s", 
                                        name, str_val, Bstrerror(Berror));
                                ndrx_TPset_error_fmt(TPESYSTEM, "Failed to set [%s] "
                                        "to [%s]: %s", 
                                        name, str_val, Bstrerror(Berror));
                                
                                EXFAIL_OUT(ret);
                            }
                        }
                        break;
                        case EXJSONNumber:
                        {
                            long l;
                            d_val = exjson_array_get_number(array, j);
                            NDRX_LOG(log_debug, "Array j=%d, Double Value: [%lf]", j, d_val);

                            if (IS_INT(cnametyp))
                            {
                                l = round_long(d_val);
                                NDRX_LOG(log_debug, "Array j=%d, Long value: [%ld]", j, l);
                                if (EXSUCCEED!=CBvchg(cstruct, view, name, j, 
                                        (char *)&l, 0L, BFLD_LONG))
                                {
                                        NDRX_LOG(log_error, "Failed to set [%s] to [%ld]: %s", 
                                                name, l, Bstrerror(Berror));
                                        
                                        ndrx_TPset_error_fmt(TPESYSTEM, 
                                                "Failed to set [%s] to [%ld]: %s", 
                                                name, l, Bstrerror(Berror));
                                        
                                        EXFAIL_OUT(ret);
                                }
                            }
                            else if (EXSUCCEED!=CBvchg(cstruct, view, name, j, 
                                    (char *)&d_val, 0L, BFLD_DOUBLE))
                            {
                                NDRX_LOG(log_error, "Failed to set [%s] to [%lf]: %s", 
                                        name, d_val, Bstrerror(Berror));
                                
                                ndrx_TPset_error_fmt(TPESYSTEM,"Failed to set "
                                        "[%s] to [%lf]: %s", 
                                        name, d_val, Bstrerror(Berror));
                                
                                EXFAIL_OUT(ret);
                            }
                        }
                        break;
                        case EXJSONBoolean:
                        {
                            bool_val = (short)exjson_array_get_boolean(array, j);
                            NDRX_LOG(log_debug, "Array j=%d, Bool Value: [%hd]", j, bool_val);
                            if (EXSUCCEED!=CBvchg(cstruct, view, name, j, 
                                    (char *)&bool_val, 0L, BFLD_SHORT))
                            {
                                NDRX_LOG(log_error, "Failed to set [%s] to [%hd]: %s", 
                                        name, bool_val, Bstrerror(Berror));
                                
                                ndrx_TPset_error_fmt(TPESYSTEM,"Failed to set "
                                        "[%s] to [%hd]: %s", 
                                        name, bool_val, Bstrerror(Berror));
                                
                                EXFAIL_OUT(ret);
                            }
                        }
                        default:
                            NDRX_LOG(log_error, 
                                        "Unsupported array elem "
                                        "type: %d", f_type);                            
                        break;
                    }
                }

            }
            break;
            default:
            {
                NDRX_LOG(log_error, "Unsupported type: %d", f_type);
            }
            break;

        }
    }
    
out:
    /* cleanup code */
    if (NULL != root_value)
    {
        exjson_value_free(root_value);
    }

    if (EXSUCCEED!=ret && NULL!=cstruct)
    {
        tpfree(cstruct);
        cstruct = NULL;
    }
    
    
    return cstruct;
}

/* vim: set ts=4 sw=4 et smartindent: */
