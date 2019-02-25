import org.junit.Test;
import static org.junit.Assert.*;
import org.endurox.*;

/**
 * Conversations tests
 */
public class Conversations {
             
    /**
     * Perform conversations tests, with UBF buffer..
     */
    @Test
    public void convTest() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        
        boolean leaktest = false;
        int leaktestSec = 0;
        StopWatch w = new StopWatch();
        Commons c = new Commons();
        
        String leaktestSecStr = System.getenv("NDRXJ_LEAKTEST");
        
        if (null!=leaktestSecStr)
        {
            leaktestSec = Integer.parseInt(leaktestSecStr);
            leaktest = true;
            
            //Nothing to test at the moment
            if (!System.getenv("NDRXJ_LEAKTEST_NAME").equals("convTest")) {
                return;
            }
                
        }
        
        /**
         * ideally we would time terminated tests, for example 5 min...?
         * thus we need a stop watch construction to have in java..
         */
        for (int i=0; ((i<1000) || (leaktest && w.deltaSec() < leaktestSec)); i++)
        {
            c.bufferCrossConvTestX(ctx);
            
        }
        
        ctx.cleanup();
    }
    
}
