import static org.junit.Assert.*;
import org.junit.Test;
import org.endurox.*;

public class AtmiCtxTest {

  /**
   * Test object allocation, buffer alloc
   */
  @Test
  public void newCtx() {
    AtmiCtx ctx = new AtmiCtx();
    assertNotEquals(ctx.getCtx(), 0x0);
    TypedUbf ub = (TypedUbf)ctx.tpAlloc("UBF", "", 1024);
    assertNotEquals(ub, null);

    /* test sub-type NULL */
    ub = (TypedUbf)ctx.tpAlloc("UBF", null, 1024);
    assertNotEquals(ub, null);
        
  }

}
