import org.junit.Test;
import static org.junit.Assert.*;
import org.endurox.*;
import org.endurox.exceptions.UbfBNOTPRESException;
/**
 * Perform marshaling tests...
 */
public class BMarshalTest {
    
    @Test
    public void testUnMarshal() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);

        short s = 100;
        ub.Badd(test.T_SHORT_FLD, s);

        BMarshalClassSingle sing = new BMarshalClassSingle();

        System.out.println("About to list... Single");

        /* load the buffer */
        ButilTest.loadTestData1(ub);

        ub.unMarshal(sing);

        assertEquals(455, sing.tshort);
        assertEquals(455, (short)sing.tshort2);

        assertEquals(111455, sing.tlong);
        assertEquals(111455, (long)sing.tlong2);

        assertEquals(124, sing.tchar);
        assertEquals(124, (byte)sing.tchar2);

        assertEquals(33.11, sing.tfloat, 0.01);
        assertEquals(33.11, (float)sing.tfloat2, 0.01);

        assertEquals(1133.11, sing.tdouble, 0.01);
        assertEquals(1133.11, (double)sing.tdouble2, 0.01);

        assertEquals("HELLO", sing.tstring);

        assertArrayEquals(new byte[]{0,1,2,3}, sing.tcarray[0]);
        assertArrayEquals(new Byte[]{0,1,2,3}, sing.tcarray2[0]);
      
        ub.unMarshal(sing, 1);
        
        
        ub.Bprint();
        
        assertEquals(-5, sing.tshort);
        assertEquals(-5, (short)sing.tshort2);

        assertEquals(-2225, sing.tlong);
        assertEquals(-2225, (long)sing.tlong2);

        assertEquals((byte)253, (byte)sing.tchar);
        assertEquals((byte)253, (byte)sing.tchar2);

        assertEquals(-500.63, sing.tfloat, 0.01);
        assertEquals(-500.63, (float)sing.tfloat2, 0.01);

        assertEquals(-22500.63, sing.tdouble, 0.01);
        assertEquals(-22500.63, (double)sing.tdouble2, 0.01);

        assertEquals("WORLD", sing.tstring);

        assertArrayEquals(new byte[]{0,4,5,6,7}, sing.tcarray[0]);
        assertArrayEquals(new Byte[]{0,4,5,6,7}, sing.tcarray2[0]);
              
        /*
        BMarshalClassArray arr = new BMarshalClassArray();

        System.out.println("About to list... Array");
        ub.unMarshal(arr);
        */
    }
    
    /**
     * The occurrence is missing, error

    @Test(expected = org.endurox.exceptions.UbfBNOTPRESException.class)
    public void testUnMarshalNoOcc() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);

        short s = 100;
        ub.Badd(test.T_SHORT_FLD, s);

        BMarshalClassSingle sing = new BMarshalClassSingle();

        System.out.println("About to list... Single");

        ButilTest.loadTestData1(ub);
        
        ub.unMarshal(sing, 2);
        
    }
    *      */
    
    /**
     * Shall get error as field is missing in buffer
     */
    @Test(expected = org.endurox.exceptions.UbfBNOTPRESException.class)
    public void testUnMarshalMandMiss() {
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);

        BMarshalClassMand sing = new BMarshalClassMand();

        System.out.println("About to list... Single/miss");
        
        ub.unMarshal(sing);
    }
    
    /**
     * Field marked as optional and is missing, no error.
     */
    @Test
    public void testUnMarshalOptMiss() {
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);

        BMarshalClassOpt sing = new BMarshalClassOpt();

        System.out.println("About to list... Single/opt/miss -> no error");
        
        ub.unMarshal(sing);
    }
    
    /**
     * This will perform normal array tests, the exception cases, min max
     * will test in other case
     */
    @Test
    public void testUnMarshalArray() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);

        short s = 100;
        ub.Badd(test.T_SHORT_FLD, s);

        BMarshalClassArray arr = new BMarshalClassArray();

        System.out.println("About to list... Array");

        /* load the buffer */
        ButilTest.loadTestData1(ub);

        /* Print UBF buffer. */
        ub.Bprint();
        ub.unMarshal(arr);
        
        /* Short tests: */
        assertEquals(2, arr.tshort.length);
        assertEquals(455, arr.tshort[0]);
        assertEquals(-5, arr.tshort[1]);
        
        assertEquals(2, arr.tshort2.length);
        assertEquals((short)455, (short)arr.tshort2[0]);
        assertEquals((short)-5, (short)arr.tshort2[1]);
        
        /* Long tests: */
        assertEquals(2, arr.tlong.length);
        assertEquals(111455, arr.tlong[0]);
        assertEquals(-2225, arr.tlong[1]);
        
        assertEquals(2, arr.tlong2.length);
        assertEquals((long)111455, (long)arr.tlong2[0]);
        assertEquals((long)-2225, (long)arr.tlong2[1]);
        
        /* Char tests: */
        assertEquals(2, arr.tchar.length);
        assertEquals((byte)124, (byte)arr.tchar[0]);
        assertEquals((byte)253, (byte)arr.tchar[1]);
        
        assertEquals(2, arr.tchar2.length);
        assertEquals((byte)124, (byte)arr.tchar2[0]);
        assertEquals((byte)253, (byte)arr.tchar2[1]);
        
        /* float tests: */
        assertEquals(2, arr.tfloat.length);
        assertEquals((float)33.11, (float)arr.tfloat[0], 0.01);
        assertEquals((float)-500.63, (float)arr.tfloat[1], 0.01);
        
        assertEquals(2, arr.tfloat2.length);
        assertEquals((float)33.11, (float)arr.tfloat2[0], 0.01);
        assertEquals((float)-500.63, (float)arr.tfloat2[1], 0.01);
        
        /* double tests: */
        assertEquals(2, arr.tdouble.length);
        assertEquals((double)1133.11, (double)arr.tdouble[0], 0.01);
        assertEquals((double)-22500.63, (double)arr.tdouble[1], 0.01);
        
        assertEquals(2, arr.tdouble2.length);
        assertEquals((double)1133.11, (double)arr.tdouble2[0], 0.01);
        assertEquals((double)-22500.63, (double)arr.tdouble2[1], 0.01);
        
        /* string tests: */
        assertEquals(3, arr.tstring.length);
        assertEquals("HELLO", arr.tstring[0]);
        assertEquals("WORLD", arr.tstring[1]);
        assertEquals("OK", arr.tstring[2]);
        
        /* carray tests: */
        assertEquals(2, arr.tcarray.length);
        assertArrayEquals(new byte[]{0,1,2,3}, arr.tcarray[0]);
        assertArrayEquals(new byte[]{0,4,5,6,7}, arr.tcarray[1]);
        
        assertEquals(2, arr.tcarray2.length);
        assertArrayEquals(new Byte[]{0,1,2,3}, arr.tcarray2[0]);
        assertArrayEquals(new Byte[]{0,4,5,6,7}, arr.tcarray2[1]);
    }
    
    /**
     * Single Occurrence unmarshal
     */
    @Test
    public void testUnMarshalArraySingle() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);

        short s = 100;
        ub.Badd(test.T_SHORT_FLD, s);

        BMarshalClassArray arr = new BMarshalClassArray();

        System.out.println("About to list... Array");

        /* load the buffer */
        ButilTest.loadTestData1(ub);

        /* Print UBF buffer. */
        ub.Bprint();
        ub.unMarshal(arr, 1);
        
        /* Short tests: */
        assertEquals(1, arr.tshort.length);
        assertEquals(-5, arr.tshort[0]);
        
        assertEquals(1, arr.tshort2.length);
        assertEquals((short)-5, (short)arr.tshort2[0]);
        
        /* Long tests: */
        assertEquals(1, arr.tlong.length);
        assertEquals(-2225, arr.tlong[0]);
        
        assertEquals(1, arr.tlong2.length);
        assertEquals((long)-2225, (long)arr.tlong2[0]);
        
        /* Char tests: */
        assertEquals(1, arr.tchar.length);
        assertEquals((byte)253, (byte)arr.tchar[0]);
        
        assertEquals(1, arr.tchar2.length);
        assertEquals((byte)253, (byte)arr.tchar2[0]);
        
        /* float tests: */
        assertEquals(1, arr.tfloat.length);
        assertEquals((float)-500.63, (float)arr.tfloat[0], 0.01);
        
        assertEquals(1, arr.tfloat2.length);
        assertEquals((float)-500.63, (float)arr.tfloat2[0], 0.01);
        
        /* double tests: */
        assertEquals(1, arr.tdouble.length);
        assertEquals((double)-22500.63, (double)arr.tdouble[0], 0.01);
        
        assertEquals(1, arr.tdouble2.length);
        assertEquals((double)-22500.63, (double)arr.tdouble2[0], 0.01);
        
        /* string tests: */
        assertEquals(1, arr.tstring.length);
        assertEquals("WORLD", arr.tstring[0]);
        
        /* carray tests: */
        assertEquals(1, arr.tcarray.length);
        assertArrayEquals(new byte[]{0,4,5,6,7}, arr.tcarray[0]);
        
        assertEquals(1, arr.tcarray2.length);
        assertArrayEquals(new Byte[]{0,4,5,6,7}, arr.tcarray2[0]);
    }
    
    /**
     * Test array marshaller, null ptr exception in array
     */
    @Test(expected = java.lang.NullPointerException.class)
    public void testMarshalArrayNullExcpetion() {
        
        BMarshalClassArray a = BMarshalClassArray.getTestData();
                
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);
       
        a.tshort2[2] = null;
        ub.marshal(a);
    }
    
    /**
     * Test array marshaller
     */
    @Test
    public void testMarshalArray() {
        
        BMarshalClassArray a = BMarshalClassArray.getTestData();
                
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);
        
        ub.marshal(a);
        
        ub.Bprint();
        
        /* test the values with expressions (will be simpler)... */
        
        /* test short: */
        assertEquals(ub.Bqboolev("T_SHORT_FLD==155 && T_SHORT_FLD[1]==667 && "+
                "T_SHORT_FLD[2]==-225 && !T_SHORT_FLD[3]" ), true);
        assertEquals(ub.Bqboolev("T_SHORT_2_FLD==-155 && T_SHORT_2_FLD[1]==0 && "+
                "T_SHORT_2_FLD[2]==225 && T_SHORT_2_FLD[3]==2 && !T_SHORT_2_FLD[4]" ), true);
        
        /* test long fields: */
        assertEquals(ub.Bqboolev("T_LONG_FLD==100000000 && T_LONG_FLD[1]==-1000000000 && "+
                "!T_LONG_FLD[2]" ), true);
        assertEquals(ub.Bqboolev("T_LONG_2_FLD==-99999999 && !T_LONG_2_FLD[1]"), true);
        
        /* test char fields: */
        assertEquals(ub.Bqboolev("T_CHAR_FLD=='A' && !T_CHAR_FLD[1]" ), true);
        assertEquals(ub.Bqboolev("T_CHAR_2_FLD=='B' && T_CHAR_2_FLD[1]==0 && !T_CHAR_FLD[2]" ), true);
        
        /* Float tests: */
        assertEquals(ub.Bqboolev("T_FLOAT_FLD==-11.22" ), true);
        assertEquals(ub.Bqboolev("T_FLOAT_2_FLD==15.4 && T_FLOAT_2_FLD[1]==-1.99" ), true);
        
        /* Double tests: */
        assertEquals(ub.Bqboolev("T_DOUBLE_FLD==-1111.22" ), true);
        assertEquals(ub.Bqboolev("T_DOUBLE_2_FLD==15511.4 && T_DOUBLE_2_FLD[1]==-1011.99" ), true);
        
        /* String tests: */
        assertEquals(ub.Bqboolev("T_STRING_FLD=='hello' && T_STRING_FLD[1]=='world'" ), true);
        
        /* carrays will extract from the buffer for testing... */
        byte[] b1 = ub.BgetByteArr(test.T_CARRAY_FLD, 0);
        assertArrayEquals(new byte[]{0,1,2,3}, b1);
        
        byte[] b1_2 = ub.BgetByteArr(test.T_CARRAY_FLD, 1);
        assertArrayEquals(new byte[]{3,2,1,3}, b1_2);
        
        byte[] b2 = ub.BgetByteArr(test.T_CARRAY_2_FLD, 0);
        assertArrayEquals(new byte[]{0,1,2,3}, b2);
        
        byte[] b2_2 = ub.BgetByteArr(test.T_CARRAY_2_FLD, 1);
        assertArrayEquals(new byte[]{3,2,1,3}, b2_2);
        
    }
    
    /**
     * Perform marshal of single entry
     */
    @Test
    public void testMarshal() {
        
        System.out.println("************** testMarshal START");
        
        BMarshalClassSingle a = BMarshalClassSingle.getTestData();
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);
        
        ub.marshal(a);
        
        ub.Bprint();
        
        /* test the values with expressions (will be simpler)... */
        
        /* test short: */
        assertEquals(true, ub.Bqboolev("T_SHORT_FLD==100 && T_SHORT_2_FLD==-100" ));
        
        /* test long: */
        assertEquals(true, ub.Bqboolev("T_LONG_FLD==10009991 && T_LONG_2_FLD==-10009991" ));
        
        /* test char: */
        assertEquals(true, ub.Bqboolev("T_CHAR_FLD=='F' && T_CHAR_2_FLD=='A'" ));
        
        /* test float: */
        assertEquals(true, ub.Bqboolev("T_FLOAT_FLD==10.1 && T_FLOAT_2_FLD==-10.1" ));
        
        /* test double: */
        assertEquals(true, ub.Bqboolev("T_DOUBLE_FLD==100.122 && T_DOUBLE_2_FLD==-100.122" ));
        
        /* test string: */
        assertEquals(true, ub.Bqboolev("T_STRING_FLD=='hello world'" ));
        
        /* byte array/ carray tests */
        byte[] b1 = ub.BgetByteArr(test.T_CARRAY_FLD, 0);
        assertArrayEquals(new byte[]{0,1,2,3}, b1);
        
        byte[] b2 = ub.BgetByteArr(test.T_CARRAY_2_FLD, 0);
        assertArrayEquals(new byte[]{3,2,1,0,-127}, b2);
        
        
    }
    
    /**
     * Perform marshaling of single array instance
     * if field is NULL, we shall skip it not ?
     */
    @Test
    public void testMarshalArraySingle() {
        
        BMarshalClassArray a = BMarshalClassArray.getTestData();
                
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);
        
        ub.marshal(a, 1);
        ub.Bprint();
        
        
        /* test the values with expressions (will be simpler)... */
        
        /* test short: */
        assertEquals(true, ub.Bqboolev("T_SHORT_FLD==667" ));
        assertEquals(true, ub.Bqboolev("T_SHORT_2_FLD==0" ));
        
        /* test long fields: */
        assertEquals(true, ub.Bqboolev("T_LONG_FLD==-1000000000"));
        assertEquals(true, ub.Bqboolev("!T_LONG_2_FLD"));
        
        /* test char fields: */
        assertEquals(true, ub.Bqboolev("!T_CHAR_FLD" ));
        assertEquals(true, ub.Bqboolev("T_CHAR_2_FLD==0" ));
        
        /* Float tests: */
        /* due to not boxed, we have 0 in "NULL" position ... */
        assertEquals(true, ub.Bqboolev("T_FLOAT_FLD==0" ));
        assertEquals(true, ub.Bqboolev("T_FLOAT_2_FLD==-1.99" ));
        
        /* Double tests: */
        assertEquals(true, ub.Bqboolev("T_DOUBLE_FLD==0" ));
        assertEquals(true, ub.Bqboolev("T_DOUBLE_2_FLD==-1011.99" ));
        
        /* String tests: */
        assertEquals(true, ub.Bqboolev("T_STRING_FLD=='world'" ));
        
        /* carrays will extract from the buffer for testing... */
        byte[] b1_2 = ub.BgetByteArr(test.T_CARRAY_FLD, 0);
        assertArrayEquals(new byte[]{3,2,1,3}, b1_2);
        
        byte[] b2_2 = ub.BgetByteArr(test.T_CARRAY_2_FLD, 0);
        assertArrayEquals(new byte[]{3,2,1,3}, b2_2);
        
    }
    
    /**
     * Perform different mandatory/optional tests for marshaling
     */
    @Test
    public void testMarshalMandFail() {
        
        boolean gotex;
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub;
                
        BMarshalClassMandOpt2 obj = new BMarshalClassMandOpt2();
        
        /* test that field missing due to mandatroy fields */
        gotex = false;
        ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        try
        {
            ub.marshal(obj);
        }
        catch (UbfBNOTPRESException e)
        {
            gotex = true;
        }
        assertEquals(true, gotex);
        
        /* arrays are OK, but singles shall fail */
        obj.tshort = new short[2];
        obj.tshort[0] = 1;
        obj.tshort[1] = 2;
        
        obj.tshort2 = new Short[2];
        obj.tshort2[0] = 2;
        obj.tshort2[1] = 3;
        
        gotex = false;
        ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        try
        {
            ub.marshal(obj);
        }
        catch (UbfBNOTPRESException e)
        {
            gotex = true;
        }
        assertEquals(true, gotex);
        
        /* arrays are OK, singles are ok now too.... */
        obj.tlong = 125L;
        obj.tstring2 = "HELLO";
        
        gotex = false;
        ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        try
        {
            ub.marshal(obj);
        }
        catch (UbfBNOTPRESException e)
        {
            e.printStackTrace();
            gotex = true;
        }
        assertEquals(false, gotex);
        
        /* validate buffer... */
        assertEquals(true, ub.Bqboolev("T_SHORT_FLD==1 && T_SHORT_FLD[1]==2"));
        assertEquals(true, ub.Bqboolev("T_SHORT_2_FLD==2 && T_SHORT_2_FLD[1]==3"));
        assertEquals(true, ub.Bqboolev("!T_STRING_FLD"));
        assertEquals(true, ub.Bqboolev("T_LONG_FLD==125"));
        assertEquals(true, ub.Bqboolev("T_STRING_2_FLD=='HELLO'"));
        
        /* Ok remove some array elems, expect reject too */
        gotex = false;
        ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        
        obj.tshort = new short[1];
        obj.tshort[0] = 1;
        
        try
        {
            ub.marshal(obj);
        }
        catch (UbfBNOTPRESException e)
        {
            //e.printStackTrace();
            gotex = true;
        }
        assertEquals(true, gotex);
        
        /* marshal extra, fine */
        gotex = false;
        ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        
        obj.tshort = new short[3];
        obj.tshort[0] = 1;
        obj.tshort[1] = 2;
        obj.tshort[2] = 3;
        
        try
        {
            ub.marshal(obj);
        }
        catch (UbfBNOTPRESException e)
        {
            e.printStackTrace();
            gotex = true;
        }
        assertEquals(false, gotex);
        assertEquals(true, ub.Bqboolev("T_SHORT_FLD==1 && T_SHORT_FLD[1]==2 "
                + "&& T_SHORT_FLD[2]==3"));
        
        /* test optional string */
        assertEquals(true, ub.Bqboolev("!T_STRING_FLD"));
        
        gotex = false;
        ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        
        obj.tstring = new String[4];
        obj.tstring[0] = "hello enduro";
        obj.tstring[1] = "hello x";
        obj.tstring[3] = "zzz";
                
        try
        {
            ub.marshal(obj);
        }
        catch (UbfBNOTPRESException e)
        {
            e.printStackTrace();
            gotex = true;
        }
        assertEquals(false, gotex);
        /* check that we stop at nulls.. */
        assertEquals(true, ub.Bqboolev("T_STRING_FLD=='hello enduro' && "
                + "T_STRING_FLD[1]='hello x' "
                + "!T_STRING_FLD[2]"));
        
    }
    
    /**
     * Perform Un-marshal tests
     */
    @Test
    public void testUnMarshalMandFail() {
        
        boolean gotex;
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        
        BMarshalClassMandOpt2 obj;
        
        /* try empty shall be exception due to missing fields */
        gotex = false;
        obj = new BMarshalClassMandOpt2();
        try
        {
            ub.unMarshal(obj);
        }
        catch (UbfBNOTPRESException e)
        {
            //e.printStackTrace();
            gotex = true;
        }
        assertEquals(true, gotex);
        
        /* fill up some fields, try again*/
        ub.Badd(test.T_SHORT_FLD, 445);
        ub.Badd(test.T_SHORT_2_FLD, 777);
        ub.Badd(test.T_LONG_FLD, 12777L);
        
        /* try empty shall be exception due to missing fields, one T_SHORT_FLD missing */
        gotex = false;
        obj = new BMarshalClassMandOpt2();
        try
        {
            ub.unMarshal(obj);
        }
        catch (UbfBNOTPRESException e)
        {
            //e.printStackTrace();
            gotex = true;
        }
        assertEquals(true, gotex);
        
        
        /* now ok, shall be all */
        
        ub.Bprint();
        
        ub.Badd(test.T_SHORT_FLD, 4);
        ub.Badd(test.T_STRING_2_FLD, "Hello world");
        gotex = false;
        obj = new BMarshalClassMandOpt2();
        try
        {
            ub.unMarshal(obj);
        }
        catch (UbfBNOTPRESException e)
        {
            e.printStackTrace();
            gotex = true;
        }
        
        assertEquals(false, gotex);
        
        assertEquals(2, obj.tshort.length);
        assertEquals(445, obj.tshort[0]);
        assertEquals(4, obj.tshort[1]);
        
        assertEquals(1, obj.tshort2.length);
        assertEquals(777, (short)obj.tshort2[0]);
        
        assertEquals(12777L, (long)obj.tlong);
        assertEquals(0, obj.tstring.length);
        
        assertEquals("Hello world", obj.tstring2);
        
        /* remove long field shall fail... */
        ub.Bdel(test.T_LONG_FLD, 0);
        obj = new BMarshalClassMandOpt2();
        try
        {
            ub.unMarshal(obj);
        }
        catch (UbfBNOTPRESException e)
        {
            //e.printStackTrace();
            gotex = true;
        }
        
        assertEquals(true, gotex);
        
        /* try the null in middle of array */
        
        ub.Badd(test.T_STRING_FLD, "str 1");
        ub.Badd(test.T_STRING_FLD, "str 2");
        ub.Badd(test.T_STRING_FLD, "str 3");
        ub.Badd(test.T_LONG_FLD, 12777L);
        obj = new BMarshalClassMandOpt2();
        gotex = false;
        try
        {
            ub.unMarshal(obj);
        }
        catch (UbfBNOTPRESException e)
        {
            e.printStackTrace();
            gotex = true;
        }
        
        assertEquals(false, gotex);
        assertEquals(3, obj.tstring.length);
        assertEquals("str 1", obj.tstring[0]);
        assertEquals("str 2", obj.tstring[1]);
        assertEquals("str 3", obj.tstring[2]);
        
    }
}

