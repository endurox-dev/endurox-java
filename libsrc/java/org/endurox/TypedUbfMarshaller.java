/**
 * @brief UBF Marshaller Engine
 *
 * @class TypedUbfMarshaller
 */
/* -----------------------------------------------------------------------------
 * Enduro/X Middleware Platform for Distributed Transaction Processing
 * Copyright (C) 2009-2016, ATR Baltic, Ltd. All Rights Reserved.
 * Copyright (C) 2017-2018, Mavimax, Ltd. All Rights Reserved.
 * This software is released under one of the following licenses:
 * GPL or Mavimax's license for commercial use.
 * -----------------------------------------------------------------------------
 * GPL license:
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 3 of the License, or (at your option) any later
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
package org.endurox;

import com.sun.xml.internal.ws.util.StringUtils;
import java.lang.reflect.Field;

public class TypedUbfMarshaller {
    
    
    
    /**
    * Capitalizes the field name unless one of the first two characters are uppercase. This is in accordance with java
    * bean naming conventions in JavaBeans API spec section 8.8.
    *
    * @param fieldName
    * @return the capitalised field name
    * @see Introspector#decapitalize(String)
    */
   public static String capatalizeFieldName(String fieldName) {
       final String result;
       if (fieldName != null && !fieldName.isEmpty()
               && Character.isLowerCase(fieldName.charAt(0))
               && (fieldName.length() == 1 || Character.isLowerCase(fieldName.charAt(1)))) {
           result = StringUtils.capitalize(fieldName);
       } else {
           result = fieldName;
       }
       return result;
   }

    /**
     * Unmarshal engine copy data from UBF to local struct
     * @param o object to process
     * @param occ -1 all occurrences, >=0 specified occurrence 
     */
    static void unmarshal(Object o, int occ) {
        
        Field[] fields = o.getClass().getDeclaredFields();
        
        for (Field field : fields) {
            
            if (field.isAnnotationPresent(UbfField.class)) {
                UbfField fAnno = field.getAnnotation(UbfField.class);
                
                int occsProc = 0;
                
                /* process annotation... */
                
                /* TODO: Get the setter of the field */
                
                /* Check the field type  
                field.getType()
                we shall get the instance of the field.
                Also if it is array, then we shall create number of array
                elements which matches the occurrences.
                */
                if (field.getType().getName().equals(o))
                {
                    
                }
                
            }
        }
    }
}

/* vim: set ts=4 sw=4 et smartindent: */
