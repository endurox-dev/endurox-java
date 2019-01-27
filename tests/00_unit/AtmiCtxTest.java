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
      
    for (int i=0; i<100; i++) {
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
  
  /**
   * Test Btype method
   */
  @Test
  public void testBtype() {
    AtmiCtx ctx = new AtmiCtx();
    assertNotEquals(0x0, ctx.getCtx());
    
    assertEquals("short", ctx.Btype(test.T_SHORT_FLD));
    assertEquals("long", ctx.Btype(test.T_LONG_FLD));
    assertEquals("char", ctx.Btype(test.T_CHAR_FLD));
    assertEquals("float", ctx.Btype(test.T_FLOAT_FLD));
    assertEquals("double", ctx.Btype(test.T_DOUBLE_FLD));
    assertEquals("string", ctx.Btype(test.T_STRING_FLD));
    assertEquals("carray", ctx.Btype(test.T_CARRAY_FLD));
  }
  
  /* todo: tests for failed Btype/Btype */

}
