import static org.junit.Assert.*;
import org.junit.Test;
import org.endurox.*;
import org.endurox.exceptions.*;

public class AtmiCtxTest {

  /**
   * Test object allocation, buffer alloc
   */
  @Test
  public void newCtx() {
      
    for (int i=0; i<10000; i++) {
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);

        /* test sub-type NULL */
        ub = (TypedUbf)ctx.tpalloc("UBF", null, 1024);
        assertNotEquals(ub, null);
        
        ub.cleanup();
        ctx.cleanup();
    }
    
  }

  /**
   * Test invalid buffer type exception
   */
  @Test(expected = org.endurox.exceptions.AtmiTPEINVALException.class)
  public void testInvalidBuffer() {
    AtmiCtx ctx = new AtmiCtx();
    assertNotEquals(ctx.getCtx(), 0x0);
    TypedUbf ub = (TypedUbf)ctx.tpalloc(null, "", 1024);
  }

}
