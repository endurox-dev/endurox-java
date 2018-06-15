import static org.junit.Assert.*;
import org.junit.Test;
import org.endurox.*;

public class AtmiCtxTest {

  /**
   * Test object allocation and logging 
   */
  @Test
  public void newCtx() {
    AtmiCtx ctx = new AtmiCtx();
    assertNotEquals(ctx.getCtx(), 0x0);
  }

}
