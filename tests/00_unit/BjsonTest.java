import org.junit.Test;
import static org.junit.Assert.*;
import org.endurox.*;

/**
 * Buffer field delete tests
 */
public class BjsonTest {
             
    /**
     * Delete single field test
     */
    @Test
    public void testTpUBFToJSON() {
        
        for (int i=0; i<100; i++)
        {
            AtmiCtx ctx = new AtmiCtx();
            assertNotEquals(ctx.getCtx(), 0x0);

            TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
            assertNotEquals(ub, null);
            
            /* Load some test data in UBF */
            ButilTest.loadTestData1(ub);
            
            /* generate some json */
            String json = ub.TpUBFToJSON();
                    
            ctx.tplog(AtmiConstants.LOG_DEBUG, false, "Got json: %s", json);
            
            ctx.cleanup();
            ub.cleanup();
        }
    }    
}
