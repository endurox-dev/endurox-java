import java.io.IOException;
import java.io.OutputStream;
import java.util.concurrent.TimeUnit;
import org.junit.Test;
import static org.junit.Assert.*;
import org.endurox.*;

/**
 * Buffer field delete tests
 */
public class TpacallTests {
             
    /**
     * Perform tpacall test
     */
    @Test
    public void tpacallTest() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);

        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);

        ub.Bchg(test.T_STRING_FLD, 0, "HELLO WORLD");
        
        ub.cleanup();
        
        ctx.cleanup();
        
    }
    
}
