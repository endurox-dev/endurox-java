import java.io.BufferedReader;
import java.io.IOException;
import java.io.Reader;
import java.nio.CharBuffer;
import static org.junit.Assert.*;
import org.junit.Test;
import org.endurox.*;
import java.util.Locale;

/**
 * Utility method tests
 */
public class ButilTest {
    
    /**
     * Test is buffer UBF?
     */
    @Test
    public void testBisubf() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(null, ub);
        assertEquals(true, ub.Bisubf());
    }

}
  
