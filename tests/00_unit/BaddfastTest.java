import static org.junit.Assert.*;
import org.junit.Test;
import org.endurox.*;
import org.endurox.exceptions.UbfBNOTPRESException;

public class BaddfastTest {
    
    /**
     * Test field adding to UBF buffer, short type
     */
    @Test
    public void testBaddfastShort() {
      AtmiCtx ctx = new AtmiCtx();
      assertNotEquals(ctx.getCtx(), 0x0);
      BFldLocInfo loc = new BFldLocInfo();
      
      TypedUBF ub = (TypedUBF)ctx.tpalloc("UBF", "", 1024);
      assertNotEquals(ub, null);

      ub.Baddfast(test.T_SHORT_FLD, (short)100, loc);
      ub.Baddfast(test.T_SHORT_FLD, (short)200, loc);
      ub.Baddfast(test.T_SHORT_FLD, (short)300, loc);
      ub.Baddfast(test.T_SHORT_FLD, (short)400, loc);

      /* print some buffer */
      ub.Bprint();

      /* get the field value and verify it */
      assertEquals(100, ub.BgetShort(test.T_SHORT_FLD, 0));
      assertEquals(200, ub.BgetShort(test.T_SHORT_FLD, 1));
      assertEquals(300, ub.BgetShort(test.T_SHORT_FLD, 2));
      assertEquals(400, ub.BgetShort(test.T_SHORT_FLD, 3));
    }
    
  
}
