import java.io.IOException;
import java.io.OutputStream;
import java.util.concurrent.TimeUnit;
import org.junit.Test;
import static org.junit.Assert.*;
import org.endurox.*;

/**
 * Buffer projection tests
 */
public class BProjTest {
             
    /**
     * Delete single field test
     */
    @Test
    public void testBproj() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);
        
        for (int i=0; i<100; i++)
        {
            ButilTest.loadTestData1(ub);
            
            ub.Bproj(new int[]{test.T_STRING_FLD, test.T_SHORT_FLD});
            
            assertEquals(true, ub.Bpres(test.T_SHORT_FLD, 0));
            assertEquals(true, ub.Bpres(test.T_SHORT_FLD, 1));
            assertEquals(false, ub.Bpres(test.T_SHORT_FLD, 2));
            
            assertEquals(false, ub.Bpres(test.T_LONG_FLD, 0));
            assertEquals(false, ub.Bpres(test.T_CHAR_FLD, 0));
            assertEquals(false, ub.Bpres(test.T_FLOAT_FLD, 0));
            assertEquals(false, ub.Bpres(test.T_DOUBLE_FLD, 0));
            
            assertEquals(true, ub.Bpres(test.T_STRING_FLD, 0));
            assertEquals(true, ub.Bpres(test.T_STRING_FLD, 1));
            assertEquals(true, ub.Bpres(test.T_STRING_FLD, 2));
            assertEquals(false, ub.Bpres(test.T_STRING_FLD, 3));
            
            assertEquals(false, ub.Bpres(test.T_CARRAY_FLD, 0));
            
        }
    }
    
}
