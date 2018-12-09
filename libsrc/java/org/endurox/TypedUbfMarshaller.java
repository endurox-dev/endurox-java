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
package org.endurox;

import java.beans.IntrospectionException;
import java.beans.PropertyDescriptor;
import org.endurox.exceptions.UbfBNOTPRESException;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import org.endurox.exceptions.UbfBEUNIXException;
import org.endurox.exceptions.UbfBSYNTAXException;

public class TypedUbfMarshaller {
    
    /**
     * Set the field value
     * @param obj object on which to operate
     * @param variableName
     * @param variableValue 
     */
    private static void invokeSetter(Object obj, String variableName, Object variableValue) {
      /* variableValue is Object because value can be an Object, Integer, String, etc... */
      try {
        /**
         * Get object of PropertyDescriptor using variable name and class
         * Note: To use PropertyDescriptor on any field/variable, 
         * the field must have both `Setter` and `Getter` method.
         */
         PropertyDescriptor objPropertyDescriptor = new PropertyDescriptor(variableName, 
                 obj.getClass());
         /* Set field/variable value using getWriteMethod() */
         objPropertyDescriptor.getWriteMethod().invoke(obj, variableValue);
      } 
      catch (IllegalAccessException e) {
        throw new UbfBEUNIXException(String.format("Got IllegalAccessException "+
                "while setting [%s] field", variableName), e);
      } catch (IllegalArgumentException e) {
        throw new UbfBEUNIXException(String.format("Got IllegalArgumentException "+
                "while setting [%s] field", variableName), e);
      } catch (InvocationTargetException e) {
        throw new UbfBEUNIXException(String.format("Got InvocationTargetException "+
                "while setting [%s] field", variableName), e);
      } catch (IntrospectionException e) {
        throw new UbfBEUNIXException(String.format("Got IntrospectionException "+
                "while setting [%s] field", variableName), e);
      }
      
   }
    
    /**
     * Copy UBF to object
     * @param o object to process
     * @param occ -1 all occurrences, >=0 specified occurrence 
     */
    static void unmarshal(Object o, int occ, TypedUbf ub) {
        
        int occi;
        int occsProc = 0;
        int occStart;
        int occStop;
        int occs;
        int minFlds;
        int totalEls;
        
        Field[] fields = o.getClass().getDeclaredFields();
        
        for (Field field : fields) {
            if (field.isAnnotationPresent(UbfField.class)) {
                UbfField fAnno = field.getAnnotation(UbfField.class);
                        
                /* process annotation... */
                
                /* TODO: Get the setter of the field 
                 * TODO: Needs array implementation for all elms
                 */
                
                /* Check the field type  
                field.getType()
                we shall get the instance of the field.
                Also if it is array, then we shall create number of array
                elements which matches the occurrences.
                */
                
                minFlds = fAnno.ubfmin();
                
                if (-1==occ)
                {
                    occs = ub.Boccur(fAnno.bfldid());
                    occStart = 0;
                    occStop = occs;
                }
                else
                {
                    occStart = occ;
                    occStop = occ+1; /* just one element */
                    
                    /* here if MIN was 0, then leave 0,
                     * if MIN > 0, then we require 1
                     */
                    
                    if (minFlds > 1) {
                        minFlds = 1;
                    }
                }
                
                String fldtyp = field.getType().getName();
                
                System.out.println("YOPT111111111111111111");
                
                System.out.println(fldtyp);
                   
                System.out.println("YOPT111111111111111111, END");
                
                totalEls = occStop - occStart;
                
                if (fldtyp.equals("[S")) {
                    
                        /* allocate the array, load fields and set the data */
                        
                        short [] arr = new short[occStop];
                        
                        for (occi=occStart; occi<occStop; occi++)
                        {
                            occsProc++;
                            arr[occi] = ub.BgetShort(fAnno.bfldid(), occi);
                        }
                        
                        invokeSetter(o, field.getName(), arr);
                }
                else if (fldtyp.equals("[Ljava.lang.Short;"))
                {
                        /* allocate the array, load fields and set the data */
                        
                        Short [] arr = new Short[occStop];
                        
                        for (occi=occStart; occi<occStop; occi++)
                        {
                            occsProc++;
                            arr[occi] = ub.BgetShort(fAnno.bfldid(), occi);
                        }
                        
                        invokeSetter(o, field.getName(), arr);
                }
                else if (fldtyp.equals("[J")) {
                         
                        /* allocate the array, load fields and set the data */
                        
                        long [] arr = new long[occStop];
                        
                        for (occi=occStart; occi<occStop; occi++)
                        {
                            arr[occi] = ub.BgetLong(fAnno.bfldid(), occi);
                        }
                        
                        invokeSetter(o, field.getName(), arr);
                }
                else if (fldtyp.equals("[Ljava.lang.Long;")) {
                         
                        /* allocate the array, load fields and set the data */
                        Long [] arr = new Long[occStop];
                        
                        for (occi=occStart; occi<occStop; occi++)
                        {
                            arr[occi] = ub.BgetLong(fAnno.bfldid(), occi);
                        }
                        
                        invokeSetter(o, field.getName(), arr);
                }
                else if (fldtyp.equals("[B")) {
                        
                        /* allocate the array, load fields and set the data */
                        byte [] arr = new byte[occStop];
                        
                        for (occi=occStart; occi<occStop; occi++)
                        {
                            arr[occi] = ub.BgetByte(fAnno.bfldid(), occi);
                        }
                        
                        invokeSetter(o, field.getName(), arr);
                }
                else if (fldtyp.equals("[Ljava.lang.Byte;")) {

                    /* allocate the array, load fields and set the data */
                    Byte [] arr = new Byte[occStop];

                    for (occi=occStart; occi<occStop; occi++)
                    {
                        arr[occi] = ub.BgetByte(fAnno.bfldid(), occi);
                    }

                    invokeSetter(o, field.getName(), arr);
                }
                else if (fldtyp.equals("[F")) {
                         
                        /* allocate the array, load fields and set the data */
                        
                        float [] arr = new float[occStop];
                        
                        for (occi=occStart; occi<occStop; occi++)
                        {
                            arr[occi] = ub.BgetFloat(fAnno.bfldid(), occi);
                        }
                        
                        invokeSetter(o, field.getName(), arr);
                }
                else if (fldtyp.equals("[Ljava.lang.Float;")) {
                         
                        /* allocate the array, load fields and set the data */
                        
                        Float [] arr = new Float[occStop];
                        
                        for (occi=occStart; occi<occStop; occi++)
                        {
                            arr[occi] = ub.BgetFloat(fAnno.bfldid(), occi);
                        }
                        
                        invokeSetter(o, field.getName(), arr);
                }
                else if (fldtyp.equals("[D")) {
                         
                        /* allocate the array, load fields and set the data */
                        
                        double [] arr = new double[occStop];
                        
                        for (occi=occStart; occi<occStop; occi++)
                        {
                            arr[occi] = ub.BgetDouble(fAnno.bfldid(), occi);
                        }
                        
                        invokeSetter(o, field.getName(), arr);
                }
                else if (fldtyp.equals("[Ljava.lang.Double;")) {
                         
                        /* allocate the array, load fields and set the data */
                        
                        Double [] arr = new Double[occStop];
                        
                        for (occi=occStart; occi<occStop; occi++)
                        {
                            arr[occi] = ub.BgetDouble(fAnno.bfldid(), occi);
                        }
                        
                        invokeSetter(o, field.getName(), arr);
                }
                else if (fldtyp.equals("[Ljava.lang.String;")) {
                         
                        /* allocate the array, load fields and set the data */
                        
                        String [] arr = new String[occStop];
                        
                        for (occi=occStart; occi<occStop; occi++)
                        {
                            arr[occi] = ub.BgetString(fAnno.bfldid(), occi);
                        }
                        
                        invokeSetter(o, field.getName(), arr);
                }
                /* We cannot distinguish between carray
                 * and byte array of single entries.
                 * thus if array of single bytes needs to read
                 * needs to use som short or long for data storage.
                 */
                else if (fldtyp.equals("[[B")) {
                         
                        /* allocate the array, load fields and set the data */
                        
                        byte [][] arr = new byte[occStop][];
                        
                        for (occi=occStart; occi<occStop; occi++)
                        {
                            arr[occi] = ub.BgetByteArr(fAnno.bfldid(), occi);
                        }
                        
                        invokeSetter(o, field.getName(), arr);
                }
                else if (fldtyp.equals("[[Ljava.lang.Byte;")) {
                         
                        /* allocate the array, load fields and set the data */
                        
                        Byte [][] arr = new Byte[occStop][];
                        
                        for (occi=occStart; occi<occStop; occi++)
                        {
                            byte[] tmp = ub.BgetByteArr(fAnno.bfldid(), occi);
                            Byte[] ba = new Byte[tmp.length];

                            int i=0;
                            for(byte b: tmp)
                                ba[i++] = b;

                            arr[occi] = ba;
                        }
                        
                        invokeSetter(o, field.getName(), arr);
                }
                else for (occi=occStart; occi<occStop; occi++) {
                    
                    occsProc++;
                       
                    if (fldtyp.equals("short") || fldtyp.equals("java.lang.Short")) {
                        
                        /* get short and set */
                        short s = ub.BgetShort(fAnno.bfldid(), occi);
                        /* set field to struct */
                        invokeSetter(o, field.getName(), s);
                        break; //Just fetch first, next no where to store...
                    }
                    else if (fldtyp.equals("long")  || fldtyp.equals("java.lang.Long")) {
                        
                        /* get short and set */
                        long l = ub.BgetLong(fAnno.bfldid(), occi);
                        /* set field to struct */
                        invokeSetter(o, field.getName(), l);
                        break; //Just fetch first, next no where to store...
                    }
                    else if (fldtyp.equals("byte") || fldtyp.equals("java.lang.Byte")) {
                        
                        /* get short and set */
                        byte b = ub.BgetByte(fAnno.bfldid(), occi);
                        
                        /* set field to struct */
                        invokeSetter(o, field.getName(), b);
                        break; //Just fetch first, next no where to store...
                    }
                    else if (fldtyp.equals("float") || fldtyp.equals("java.lang.Float")) {
                        
                        /* get short and set */
                        float f = ub.BgetFloat(fAnno.bfldid(), occi);
                        /* set field to struct */
                        invokeSetter(o, field.getName(), f);
                        break; //Just fetch first, next no where to store...
                    }
                    else if (fldtyp.equals("double") || fldtyp.equals("java.lang.Double")) {
                        
                        /* get short and set */
                        double d = ub.BgetDouble(fAnno.bfldid(), occi);
                        /* set field to struct */
                        invokeSetter(o, field.getName(), d);
                        break; //Just fetch first, next no where to store...
                    }
                    else if (fldtyp.equals("java.lang.String")) {
                        
                        /* get short and set */
                        String s = ub.BgetString(fAnno.bfldid(), occi);
                        /* set field to struct */
                        invokeSetter(o, field.getName(), s);
                        break; //Just fetch first, next no where to store...
                    }
                    else
                    {
                        throw new UbfBSYNTAXException(String.format("Field type [%s] not "+
                                "supported for unmarshal op, field [%s] of class [%s]", 
                                fldtyp, field.getName(), o.getClass().toString()));
                    }
                    
                } /* for  */
                
                //check that mandatory fields succeeded?
                if (occsProc < minFlds) {
                    throw new UbfBNOTPRESException(String.format("Min fields %d, found %d "+
                                "for Object field [%s], UBF fields %d, [%s]", 
                                minFlds, occsProc, field.getName(), 
                                fAnno.bfldid(), ub.ctx.Bfname(fAnno.bfldid()  )));
                }
            } /* if have UbfField annotation */
        } /* for each field */
    }
    
}

/* vim: set ts=4 sw=4 et smartindent: */
