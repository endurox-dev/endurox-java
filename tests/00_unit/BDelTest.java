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
     * Delete single field test
     */
    @Test
    public void testBdel() {
        
        for (int i=0; i<100; i++)
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

            ctx.tplogInfo("Buffer print ----------------");
            ub.Bprint();
            ctx.tplogInfo("END ------------------------");
            
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
            
            assertEquals(true, ub.Bpres(test.T_LONG_FLD, 3));
            
            ctx.cleanup();
            ub.cleanup();
        }
    }
    
    /**
     * Test all occurrences of the field
     */
    @Test
    public void testBdelall() {
        
        for (int i=0; i<100; i++)
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

            ctx.tplogInfo("Buffer print ----------------");
            ub.Bprint();
            ctx.tplogInfo("END ------------------------");
            
            assertEquals(true, ub.Bpres(test.T_STRING_FLD, 0));
            assertEquals(true, ub.Bpres(test.T_STRING_FLD, 1));
            assertEquals(true, ub.Bpres(test.T_STRING_FLD, 2));
            assertEquals(false, ub.Bpres(test.T_STRING_FLD, 3));
            
            assertEquals(true, ub.Bpres(test.T_LONG_FLD, 0));
            assertEquals(true, ub.Bpres(test.T_LONG_FLD, 1));
            assertEquals(true, ub.Bpres(test.T_LONG_FLD, 2));
            assertEquals(true, ub.Bpres(test.T_LONG_FLD, 3));
            assertEquals(false, ub.Bpres(test.T_LONG_FLD, 4));
            
            ub.Bdelall(test.T_STRING_FLD);
            
            assertEquals(false, ub.Bpres(test.T_STRING_FLD, 0));
            assertEquals(true, ub.Bpres(test.T_LONG_FLD, 3));
            
            ctx.cleanup();
            ub.cleanup();
        }
    }
    
    
    /**
     * Delete set of the fields
     */
    @Test
    public void testBdelete() {
        
        for (int i=0; i<100; i++)
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
            
            ub.Bchg(test.T_STRING_3_FLD, 2, "HELLO MARS 1");
            ub.Bchg(test.T_STRING_4_FLD, 10, "HELLO MARS 2");

            ctx.tplogInfo("Buffer print ----------------");
            ub.Bprint();
            ctx.tplogInfo("END ------------------------");
            
            assertEquals(true, ub.Bpres(test.T_STRING_FLD, 0));
            assertEquals(true, ub.Bpres(test.T_STRING_FLD, 1));
            assertEquals(true, ub.Bpres(test.T_STRING_FLD, 2));
            assertEquals(false, ub.Bpres(test.T_STRING_FLD, 3));
            
            assertEquals(true, ub.Bpres(test.T_LONG_FLD, 0));
            assertEquals(true, ub.Bpres(test.T_LONG_FLD, 1));
            assertEquals(true, ub.Bpres(test.T_LONG_FLD, 2));
            assertEquals(true, ub.Bpres(test.T_LONG_FLD, 3));
            assertEquals(false, ub.Bpres(test.T_LONG_FLD, 4));
            
            
            assertEquals(true, ub.Bpres(test.T_STRING_3_FLD, 2));
            assertEquals(true, ub.Bpres(test.T_STRING_4_FLD, 10));
            
            
            ub.Bdelete(new int[]{test.T_STRING_4_FLD, test.T_STRING_FLD});
            
            
            assertEquals(true, ub.Bpres(test.T_LONG_FLD, 0));
            assertEquals(true, ub.Bpres(test.T_STRING_3_FLD, 0));
            
            assertEquals(false, ub.Bpres(test.T_STRING_FLD, 0));
            assertEquals(false, ub.Bpres(test.T_STRING_4_FLD, 0));
            
            ctx.cleanup();
            ub.cleanup();
        }
    }
    
}
