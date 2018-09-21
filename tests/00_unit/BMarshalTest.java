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
      assertEquals(111455, sing.tlong);
      assertEquals(124, sing.tchar);
      assertEquals(33.11, sing.tfloat, 0.01);
      assertEquals(1133.11, sing.tdouble, 0.01);
      assertEquals("HELLO", sing.tstring);
      assertArrayEquals(new byte[]{0,1,2,3}, sing.tcarray);
      
      /*
      BMarshalClassArray arr = new BMarshalClassArray();
      
      System.out.println("About to list... Array");
      ub.unMarshal(arr);
        */
    }
}
