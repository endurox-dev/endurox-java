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
import java.lang.reflect.Array;
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
      } 
      catch (IllegalArgumentException e) {
        throw new UbfBEUNIXException(String.format("Got IllegalArgumentException "+
                "while setting [%s] field", variableName), e);
      } 
      catch (InvocationTargetException e) {
        throw new UbfBEUNIXException(String.format("Got InvocationTargetException "+
                "while setting [%s] field", variableName), e);
      } 
      catch (IntrospectionException e) {
        throw new UbfBEUNIXException(String.format("Got IntrospectionException "+
                "while setting [%s] field", variableName), e);
      }
    }
    
    /**
     * Invoke field getter value
     * @param variableName variable to read value from
     */
    private static Object invokeGetter(Object obj, String variableName) {
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
         return objPropertyDescriptor.getReadMethod().invoke(obj);
      } 
      catch (IllegalAccessException e) {
        throw new UbfBEUNIXException(String.format("Got IllegalAccessException "+
                "while setting [%s] field", variableName), e);
      } 
      catch (IllegalArgumentException e) {
        throw new UbfBEUNIXException(String.format("Got IllegalArgumentException "+
                "while setting [%s] field", variableName), e);
      } 
      catch (InvocationTargetException e) {
        throw new UbfBEUNIXException(String.format("Got InvocationTargetException "+
                "while setting [%s] field", variableName), e);
      } 
      catch (IntrospectionException e) {
        throw new UbfBEUNIXException(String.format("Got IntrospectionException "+
                "while setting [%s] field", variableName), e);
      }
      
    }
    
    /**
     * Copy Object data to UBF.
     * Loop over the object meta data in set any ubf fields if found.
     * @param o object to take data from
     * @param occ array occurrence to load into buffer, or -1 to load all
     *  the data is loaded from 0 occ in UBF anyway
     * @param ub UBF buffer to load data to. It is assumed that there is
     *  enough space there.
     */
    static void marshal(Object o, int occ, TypedUbf ub) {
        int occi;
        int occsProc;
        int occStart;
        int occStop;
        int occs;
        int minFlds;
        Object fldVal;
        
        Field[] fields = o.getClass().getDeclaredFields();

        for (Field field : fields) {
            if (field.isAnnotationPresent(UbfField.class)) {
                UbfField fAnno = field.getAnnotation(UbfField.class);
                occsProc = occsProc=0;
                minFlds = fAnno.ojbmin();
                String fldtyp = field.getType().getName();
                        
                /* Detect type is it array, or what?
                 * if array, get the length 
                */
                fldVal = invokeGetter(o, field.getName());
                
                
                /* check if field is boxed and not NULL,
                    if NULL, then occs = 0 
                */
                if (null==fldVal)
                {
                    occs = 0;
                } 
                else /**
                 * For arrays we assume that all elements are filled.
                 * if we get null for boxed type, then empty value will be
                 * set in UBF
                 */
                if (field.getType().isArray()) {
                    occs = Array.getLength(fldVal);
                } 
                else {
                    occs = 1;
                }
                
                //https://docs.oracle.com/javase/6/docs/api/java/lang/reflect/Array.html
                //field.getClass()
                if (occ==-1) {
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
                
                /* pre check conditions */
                
                if ( minFlds > occs) {
                    /* Raise exception -> minimum X but in array Y */
                    throw new UbfBNOTPRESException(String.format("Minimum fields %d "
                            + "but array have %d, java field: [%s], UBF field: [%s]", 
                               minFlds, occs, field.getName(), 
                              ub.ctx.Bfname(fAnno.bfldid()  )));
                } else if ( occStop > occs ) {
                    /* max index requested: occStop-1, but have occs-1 */
                    /* Raise exception -> minimum X but in array Y */
                    
                    /* well we could just ignore this condition....
                     * and continue with next field ... or 
                     * process until the last occ available... 
                     * we we do not run in -1 mode
                     * but we are here only in case if we run in non -1 mode
                     * thus just continue;
                     */
                    
                    if (minFlds > 0)
                    {
                        throw new UbfBNOTPRESException(String.format("Range end pos %d "
                                + "but array have of max index %d, java "
                                + "field: [%s], UBF field: [%s]", 
                                  occStop-1, occs-1, field.getName(), 
                                  ub.ctx.Bfname(fAnno.bfldid()  )));
                    }
                    else
                    {
                        continue;
                    }
                }
                
                /* In case of array, access in one way */
                if (null==fldVal) {
                    /* not items in array..., just skip */
                     System.out.println(String.format("NULL HELLO =>> [%s]\n", 
                            fldtyp));
                }
                else if (field.getType().isArray()) {
                    
                    /*
                    Object[] values;
                    int arrayLen = Array.getLength(fldVal);
                    
                    System.out.println(String.format("array ty START p: (%s)\n", 
                            fldVal.getClass().getName()));
                    
                    
                    if(fldVal instanceof Object[])
                    {
                        values = (Object[])fldVal;
                    }
                    else // box primitive arrays
                    {
                        final Object[] boxedArray = new Object[arrayLen];
                        for(int index=0;index<boxedArray.length;index++)
                        {
                            boxedArray[index] = Array.get(fldVal, index); // automatic boxing
                        }
                        values = (Object[])boxedArray;
                    }
                    
                    */
                    
                    //System.out.println(String.format("array ty START p: (%s)\n", 
                    //        fldVal.getClass().getName()));
                    
                    /* process items one by one... -> load into buffer */
                    if (fldtyp.equals("[S") || fldtyp.equals("[Ljava.lang.Short;")) {
                        
                        for (occi=occStart; occi<occStop; occi++)
                        {
                            /* at which occ we will perform loading? 
                             * well only if the position is NULL?
                             * shall we put exception?
                             * I guess so...
                             */
                            Short s = (Short)Array.get(fldVal, occi);
                            
                            if (null==s)
                            {
                                break;
                            }
                            
                            ub.Bchg(fAnno.bfldid(), occi-occStart, s);
                            occsProc++;
                        }
                    } 
                    else if (fldtyp.equals("[J") || fldtyp.equals("[Ljava.lang.Long;")) {
                        
                        for (occi=occStart; occi<occStop; occi++)
                        {
                            Long l = (Long)Array.get(fldVal, occi);
                            
                            if (null==l)
                            {
                                break;
                            }
                            
                            ub.Bchg(fAnno.bfldid(), occi-occStart, l);
                            occsProc++;
                        }
                    } 
                    else if (fldtyp.equals("[B") || fldtyp.equals("[Ljava.lang.Byte;")) {
                        
                        for (occi=occStart; occi<occStop; occi++)
                        {
                            Byte b = (Byte)Array.get(fldVal, occi);
                            
                            if (null==b)
                            {
                                break;
                            }
                            
                            ub.Bchg(fAnno.bfldid(), occi-occStart, b);
                            occsProc++;
                        }
                    } 
                    else if (fldtyp.equals("[F") || fldtyp.equals("[Ljava.lang.Float;")) {
                        
                        for (occi=occStart; occi<occStop; occi++)
                        {
                            Float f = (Float)Array.get(fldVal, occi);
                            
                            if (null==f)
                            {
                                break;
                            }
                            
                            ub.Bchg(fAnno.bfldid(), occi-occStart, f);
                            occsProc++;
                        }
                    } 
                    else if (fldtyp.equals("[D") || fldtyp.equals("[Ljava.lang.Double;")) {
                        
                        for (occi=occStart; occi<occStop; occi++)
                        {
                            Double d = (Double)Array.get(fldVal, occi);
                            
                            if (null==d)
                            {
                                break;
                            }
                            
                            ub.Bchg(fAnno.bfldid(), occi-occStart, d);
                            occsProc++;
                        }
                    } 
                    else if (fldtyp.equals("[Ljava.lang.String;")) {
                        
                        for (occi=occStart; occi<occStop; occi++)
                        {
                            String s = (String)Array.get(fldVal, occi);
                            
                            if (null==s)
                            {
                                break;
                            }
                            
                            ub.Bchg(fAnno.bfldid(), occi-occStart, s);
                            occsProc++;
                        }
                    }
                    else if (fldtyp.equals("[[B")) {
                        
                        for (occi=occStart; occi<occStop; occi++)
                        {
                            /* convert array to unboxed type... */
                            byte[] tmp = (byte[])Array.get(fldVal, occi);
                            
                            if (null==tmp)
                            {
                                break;
                            }
                            
                            ub.Bchg(fAnno.bfldid(), occi-occStart, tmp);
                            occsProc++;
                        }
                    }
                    else if (fldtyp.equals("[[Ljava.lang.Byte;"))
                    {
                        for (occi=occStart; occi<occStop; occi++)
                        {
                            /* convert array to unboxed type... */
                            Byte[] tmp = (Byte[])Array.get(fldVal, occi);
                            
                            int len = Array.getLength(tmp);
                            byte [] tmpB = new byte[len];
                            
                            for (int i=0; i<len; i++)
                            {
                                tmpB[i] = (Byte)Array.get(tmp, i);
                            }
                            
                            if (null==tmp)
                            {
                                break;
                            }
                            
                            ub.Bchg(fAnno.bfldid(), occi-occStart, tmpB);
                            occsProc++;
                        }
                    }
                    
                } else {
                    
                    System.out.println(String.format("HELLO =>> [%s]\n", 
                            fldtyp));
                    
                    /* Any signle item is loaded in occ 0 */
                    if (fldtyp.equals("short") || fldtyp.equals("java.lang.Short")) {
                        
                        /* get short and set */
                        Short s = (Short)fldVal;
                        
                        if (null==s)
                        {
                            break;
                        }
                        
                        /* set field to struct */
                        ub.Bchg(fAnno.bfldid(), 0, s);
                        occsProc++;
                    }
                    else if (fldtyp.equals("long")  || fldtyp.equals("java.lang.Long")) {
                        
                        /* get long */
                        Long l = (Long)fldVal;
                        
                        if (null==l)
                        {
                            break;
                        }
                        
                        ub.Bchg(fAnno.bfldid(), 0, l);
                        occsProc++;
                    }
                    else if (fldtyp.equals("byte") || fldtyp.equals("java.lang.Byte")) {
                        
                        Byte b = (Byte)fldVal;
                        
                        if (null==b)
                        {
                            break;
                        }
                        
                        ub.Bchg(fAnno.bfldid(), 0, b);
                        occsProc++;
                    }
                    else if (fldtyp.equals("float") || fldtyp.equals("java.lang.Float")) {
                        
                        Float f = (Float)fldVal;
                        
                        if (null==f)
                        {
                            break;
                        }
                        
                        ub.Bchg(fAnno.bfldid(), 0, f);
                        occsProc++;
                    }
                    else if (fldtyp.equals("double") || fldtyp.equals("java.lang.Double")) {
                        
                        Double d = (Double)fldVal;
                        
                        if (null==d)
                        {
                            break;
                        }
                        
                        ub.Bchg(fAnno.bfldid(), 0, d);
                        occsProc++;
                    }
                    else if (fldtyp.equals("java.lang.String")) {
                        
                        String s = (String)fldVal;
                        
                        if (null==s)
                        {
                            break;
                        }
                        
                        ub.Bchg(fAnno.bfldid(), 0, s);
                        occsProc++;
                    }
                    else
                    {
                        throw new UbfBSYNTAXException(String.format("Field type [%s] not "+
                                "supported for unmarshal op, field [%s] of class [%s]", 
                                fldtyp, field.getName(), o.getClass().toString()));
                    }
                }
                
                /* In case of non array items, access in different way */
                
                if (occsProc < minFlds) {
                    throw new UbfBNOTPRESException(String.format("Min fields %d, found %d "+
                                "for Object field [%s], UBF fields %d, [%s]", 
                                minFlds, occsProc, field.getName(), 
                                fAnno.bfldid(), ub.ctx.Bfname(fAnno.bfldid()  )));
                }
            }
        } /* for each field */
    }
    
    /**
     * Copy UBF to object.
     * In case of requesting occurrence but missing in UBF, will perform
     * tests according to min fields only.
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
                occsProc = 0;
                
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
                
                //System.out.println(fldtyp);
                
                totalEls = occStop - occStart;
                
                try {
                    /**
                     * We start array unmarshal always from element 0
                     * even if specific occurrence was given.
                     */
                    if (fldtyp.equals("[S")) {

                        /* allocate the array, load fields and set the data */
                            short [] arr = new short[occStop-occStart];

                            for (occi=occStart; occi<occStop; occi++)
                            {
                                occsProc++;
                                arr[occi-occStart] = ub.BgetShort(fAnno.bfldid(), occi);
                            }

                            invokeSetter(o, field.getName(), arr);
                    }
                    else if (fldtyp.equals("[Ljava.lang.Short;"))
                    {
                        /* allocate the array, load fields and set the data */

                        Short [] arr = new Short[occStop-occStart];

                        for (occi=occStart; occi<occStop; occi++)
                        {
                            occsProc++;
                            arr[occi-occStart] = ub.BgetShort(fAnno.bfldid(), occi);
                        }

                        invokeSetter(o, field.getName(), arr);
                    }
                    else if (fldtyp.equals("[J")) {

                        /* allocate the array, load fields and set the data */

                        long [] arr = new long[occStop-occStart];

                        for (occi=occStart; occi<occStop; occi++)
                        {
                            arr[occi-occStart] = ub.BgetLong(fAnno.bfldid(), occi);
                        }

                        invokeSetter(o, field.getName(), arr);
                    }
                    else if (fldtyp.equals("[Ljava.lang.Long;")) {

                        /* allocate the array, load fields and set the data */
                        Long [] arr = new Long[occStop-occStart];

                        for (occi=occStart; occi<occStop; occi++)
                        {
                            arr[occi-occStart] = ub.BgetLong(fAnno.bfldid(), occi);
                        }

                        invokeSetter(o, field.getName(), arr);
                    }
                    else if (fldtyp.equals("[B")) {

                        byte [] arr = new byte[occStop-occStart];

                        for (occi=occStart; occi<occStop; occi++)
                        {
                            arr[occi-occStart] = ub.BgetByte(fAnno.bfldid(), occi);
                        }

                        invokeSetter(o, field.getName(), arr);
                    }
                    else if (fldtyp.equals("[Ljava.lang.Byte;")) {

                        /* allocate the array, load fields and set the data */
                        Byte [] arr = new Byte[occStop-occStart];

                        for (occi=occStart; occi<occStop; occi++)
                        {
                            arr[occi-occStart] = ub.BgetByte(fAnno.bfldid(), occi);
                        }

                        invokeSetter(o, field.getName(), arr);
                    }
                    else if (fldtyp.equals("[F")) {

                        /* allocate the array, load fields and set the data */

                        float [] arr = new float[occStop-occStart];

                        for (occi=occStart; occi<occStop; occi++)
                        {
                            arr[occi-occStart] = ub.BgetFloat(fAnno.bfldid(), occi);
                        }

                        invokeSetter(o, field.getName(), arr);
                    }
                    else if (fldtyp.equals("[Ljava.lang.Float;")) {

                        /* allocate the array, load fields and set the data */

                        Float [] arr = new Float[occStop-occStart];

                        for (occi=occStart; occi<occStop; occi++)
                        {
                            arr[occi-occStart] = ub.BgetFloat(fAnno.bfldid(), occi);
                        }

                        invokeSetter(o, field.getName(), arr);
                    }
                    else if (fldtyp.equals("[D")) {

                        /* allocate the array, load fields and set the data */

                        double [] arr = new double[occStop-occStart];

                        for (occi=occStart; occi<occStop; occi++)
                        {
                            arr[occi-occStart] = ub.BgetDouble(fAnno.bfldid(), occi);
                        }

                        invokeSetter(o, field.getName(), arr);
                    }
                    else if (fldtyp.equals("[Ljava.lang.Double;")) {

                        /* allocate the array, load fields and set the data */

                        Double [] arr = new Double[occStop-occStart];

                        for (occi=occStart; occi<occStop; occi++)
                        {
                            arr[occi-occStart] = ub.BgetDouble(fAnno.bfldid(), occi);
                        }

                        invokeSetter(o, field.getName(), arr);
                    }
                    else if (fldtyp.equals("[Ljava.lang.String;")) {

                        /* allocate the array, load fields and set the data */

                        String [] arr = new String[occStop-occStart];

                        for (occi=occStart; occi<occStop; occi++)
                        {
                            arr[occi-occStart] = ub.BgetString(fAnno.bfldid(), occi);
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

                        byte [][] arr = new byte[occStop-occStart][];

                        for (occi=occStart; occi<occStop; occi++)
                        {
                            arr[occi-occStart] = ub.BgetByteArr(fAnno.bfldid(), occi);
                        }

                        invokeSetter(o, field.getName(), arr);
                    }
                    else if (fldtyp.equals("[[Ljava.lang.Byte;")) {

                        /* allocate the array, load fields and set the data */

                        Byte [][] arr = new Byte[occStop-occStart][];

                        for (occi=occStart; occi<occStop; occi++)
                        {
                            byte[] tmp = ub.BgetByteArr(fAnno.bfldid(), occi);
                            Byte[] ba = new Byte[tmp.length];

                            int i=0;
                            for(byte b: tmp)
                                ba[i++] = b;

                            arr[occi-occStart] = ba;
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
                }
                catch (UbfBNOTPRESException ex)
                {
                    /* nothing todo, this ok, just continue */
                }
                catch (RuntimeException e) {
                    throw e;
                }
                
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
