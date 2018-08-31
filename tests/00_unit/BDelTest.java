import java.io.IOException;
import java.io.OutputStream;
import java.util.concurrent.TimeUnit;
import org.junit.Test;
import static org.junit.Assert.*;
import org.endurox.*;

/**
 * Buffer field delete tests
 */
public class BDelTest {
             
    /**
     * Test Simple version of boolean expression evaluation
     */
    @Test
    public void testBboolev() {
        
        for (int i=0; i<10000; i++)
        {
            AtmiCtx ctx = new AtmiCtx();
            assertNotEquals(ctx.getCtx(), 0x0);

            TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
            assertNotEquals(ub, null);

            ub.Bchg(test.T_STRING_FLD, 0, "HELLO WORLD");
            ub.Bchg(test.T_STRING_FLD, 1, "HELLO WORLD 1");
            ub.Bchg(test.T_STRING_FLD, 2, "HELLO WORLD 2");
            ub.Bchg(test.T_LONG_FLD, 0, 550);
            ub.Bchg(test.T_LONG_FLD, 1, 551);
            ub.Bchg(test.T_LONG_FLD, 2, 552);
            ub.Bchg(test.T_LONG_FLD, 3, 553);

            ctx.tplogError("Buffer print ----------------");
            ub.Bprint();
            ctx.tplogError("END ------------------------");
            
            assertEquals(true, ub.Bpres(test.T_STRING_FLD, 0));
            assertEquals(true, ub.Bpres(test.T_STRING_FLD, 1));
            assertEquals(true, ub.Bpres(test.T_STRING_FLD, 2));
            assertEquals(false, ub.Bpres(test.T_STRING_FLD, 3));
            
            assertEquals(true, ub.Bpres(test.T_LONG_FLD, 0));
            assertEquals(true, ub.Bpres(test.T_LONG_FLD, 1));
            assertEquals(true, ub.Bpres(test.T_LONG_FLD, 2));
            assertEquals(true, ub.Bpres(test.T_LONG_FLD, 3));
            assertEquals(false, ub.Bpres(test.T_LONG_FLD, 4));
            
            ub.Bdel(test.T_STRING_FLD, 1);
            
            assertEquals(true, ub.Bpres(test.T_STRING_FLD, 0));
            assertEquals(true, ub.Bpres(test.T_STRING_FLD, 1));
            assertEquals(false, ub.Bpres(test.T_STRING_FLD, 2));
            
            assertEquals("HELLO WORLD 2", ub.BgetString(test.T_STRING_FLD, 1));
            
            ctx.cleanup();
            ub.cleanup();
        }
        
    }
    
}
