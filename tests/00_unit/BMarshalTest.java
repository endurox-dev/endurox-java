import org.junit.Test;
import static org.junit.Assert.*;
import org.endurox.*;
import java.lang.Short;
import java.lang.Long;
import java.lang.Byte;
import java.lang.Float;
import java.lang.Double;
/**
 * Perform marshalling tests...
 */
public class BMarshalTest {
    
    //@Test
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

        assertArrayEquals(new byte[][]{{0,1,2,3}}, sing.tcarray);
        assertArrayEquals(new Byte[][]{{0,1,2,3}}, sing.tcarray2);
      
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

        assertArrayEquals(new byte[][]{{0,4,5,6,7}}, sing.tcarray);
        assertArrayEquals(new Byte[][]{{0,4,5,6,7}}, sing.tcarray2);
              
        /*
        BMarshalClassArray arr = new BMarshalClassArray();

        System.out.println("About to list... Array");
        ub.unMarshal(arr);
        */
    }
    
    /**
     * The occurrence is missing, error
     */
    //@Test(expected = org.endurox.exceptions.UbfBNOTPRESException.class)
    public void testUnMarshalNoOcc() {
        
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
        
        ub.unMarshal(sing, 2);
        
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
}
