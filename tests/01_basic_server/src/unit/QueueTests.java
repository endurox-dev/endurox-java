import org.junit.Test;
import static org.junit.Assert.*;
import org.endurox.*;
import org.endurox.exceptions.AtmiException;
import org.endurox.exceptions.UbfBNOTPRESException;

/**
 * Queue tests
 */
public class QueueTests {
             
    /**
     * Perform tpcall test
     */
    @Test
    public void basicQTest() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);

        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);
        
        boolean leaktest = false;
        int leaktestSec = 0;
        StopWatch w = new StopWatch();
        
        String leaktestSecStr = System.getenv("NDRXJ_LEAKTEST");
        
        if (null!=leaktestSecStr)
        {
            leaktestSec = Integer.parseInt(leaktestSecStr);
            leaktest = true;
            
            //Nothing to test at the moment
            if (!System.getenv("NDRXJ_LEAKTEST_NAME").equals("basicQTest")) {
                return;
            }
                
        }
        
        /**
         * TODO: Have long term test for memory management.
         * ideally we would time terminated tests, for example 5 min...?
         * thus we need a stop watch construction to have in java..
         */
        for (int i=0; ((i<1000) || (leaktest && w.deltaSec() < leaktestSec)); i++)
        {
            TPQCTL ctl = new TPQCTL();
            
            try {
                ub.Bdel(test.T_STRING_2_FLD, 0);
            } 
            catch (UbfBNOTPRESException e)
            {
                // ignore.. 
            }
            
            String reqData = String.format("loop %d", i);
            ub.Bchg(test.T_STRING_FLD, 0, String.format("loop %d", i));
            
            ctx.tpenqueue("MYSPACE", reqData, ctl, ub, 0);
            
            
            ctl = new TPQCTL();
            TypedUbf retBuf = (TypedUbf)ctx.tpdequeue("MYSPACE", reqData, ctl, ub, 0);
            
            String rspData = retBuf.BgetString(test.T_STRING_2_FLD, 0);
            assertEquals(String.format("loop %d", i), rspData);
            
            /* try to get msg... when emtpy... */            
        }
        ub.cleanup();
        ctx.cleanup();
    }
}
