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

        assertArrayEquals(new byte[]{0,1,2,3}, sing.tcarray);
        assertArrayEquals(new Byte[]{0,1,2,3}, sing.tcarray2);
      
        ub.unMarshal(sing, 1);
        
        
        ub.Bprint();
        
        assertEquals(-5, sing.tshort);
        assertEquals(-5, (short)sing.tshort2);

        assertEquals(-2225, sing.tlong);
        assertEquals(-2225, (long)sing.tlong2);

        assertEquals(253, sing.tchar);
        assertEquals(253, (byte)sing.tchar2);

        assertEquals(-500.63, sing.tfloat, 0.01);
        assertEquals(-500.63, (float)sing.tfloat2, 0.01);

        assertEquals(-22500.63, sing.tdouble, 0.01);
        assertEquals(-22500.63, (double)sing.tdouble2, 0.01);

        assertEquals("WORLD", sing.tstring);

        assertArrayEquals(new byte[]{0,4,5,6,7}, sing.tcarray);
        assertArrayEquals(new Byte[]{0,4,5,6,7}, sing.tcarray2);
              
        /*
        BMarshalClassArray arr = new BMarshalClassArray();

        System.out.println("About to list... Array");
        ub.unMarshal(arr);
        */
    }
    
    //@Test
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
      
      /*
      assertEquals(455, arr.tshort);
      assertEquals(111455, arr.tlong);
      assertEquals(124, arr.tchar);
      assertEquals(33.11, arr.tfloat, 0.01);
      assertEquals(1133.11, arr.tdouble, 0.01);
      assertEquals("HELLO", arr.tstring);
      assertArrayEquals(new byte[]{0,1,2,3}, arr.tcarray);
      */
      
      /*
      BMarshalClassArray arr = new BMarshalClassArray();
      
      System.out.println("About to list... Array");
      ub.unMarshal(arr);
        */
    }
}
