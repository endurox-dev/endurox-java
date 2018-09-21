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
    public void testBproj() {
        
      AtmiCtx ctx = new AtmiCtx();
      assertNotEquals(ctx.getCtx(), 0x0);
      TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
      assertNotEquals(ub, null);

      short s = 100;
      ub.Badd(test.T_SHORT_FLD, s);
      ub.Badd(test.T_SHORT_FLD, (short)201);
      
      
      BMarshalClassSingle sing = new BMarshalClassSingle();
      
      System.out.println("About to list... Single");
      ub.unMarshal(sing);
      
      BMarshalClassArray arr = new BMarshalClassArray();
      
      System.out.println("About to list... Array");
      ub.unMarshal(arr);
      
    }     
    
}

