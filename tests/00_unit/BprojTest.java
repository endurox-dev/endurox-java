import java.io.BufferedReader;
import java.io.IOException;
import java.io.Reader;
import java.nio.CharBuffer;
import static org.junit.Assert.*;
import org.junit.Test;
import org.endurox.*;
import java.util.Locale;

/**
 * Projection test
 */
public class BprojTest {
    
    /**
     * Test buffer copy functionality
     */
    @Test
    public void testBcpy() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(null, ub);
        
        /* load some test data.. */
        ButilTest.loadTestData1(ub);
        ButilTest.validateTestData(ub);
        
        TypedUbf ub2 = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(null, ub2);
        
        ub2.Bcpy(ub);
        ButilTest.validateTestData(ub2);
    }
    
}
  
