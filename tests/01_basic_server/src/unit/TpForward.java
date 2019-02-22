import org.junit.Test;
import static org.junit.Assert.*;
import org.endurox.*;
import org.endurox.exceptions.AtmiException;
import org.endurox.exceptions.UbfBNOTPRESException;

/**
 * Tpforward ivoker
 */
public class TpForward {
             
    /**
     * Perform test of tpforward
     */
    @Test
    public void tpforwardTest() {
        
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
            if (!System.getenv("NDRXJ_LEAKTEST_NAME").equals("tpforwardTest")) {
                return;
            }
                
        }
        
        /**
         * ideally we would time terminated tests, for example 5 min...?
         * thus we need a stop watch construction to have in java..
         */
        for (int i=0; ((i<1000) || (leaktest && w.deltaSec() < leaktestSec)); i++)
        {
            
            try {
                ub.Bdelall(test.T_STRING_FLD);
            } 
            catch (UbfBNOTPRESException e)
            {
                // ignore.. 
            }
            
            String reqData = String.format("loop %d", i);
            ub.Bchg(test.T_STRING_FLD, 0, String.format("loop %d", i));
            
            ub = (TypedUbf)ctx.tpcall("UBFOKFWD1", ub, 0);
            
            String rspData1 = ub.BgetString(test.T_STRING_FLD, 0);
            String rspData2 = ub.BgetString(test.T_STRING_FLD, 1);
            String rspData3 = ub.BgetString(test.T_STRING_FLD, 2);
            
            assertEquals(reqData, rspData1);
            assertEquals("UBF FWD 1", rspData2);
            assertEquals("UBF OK 2", rspData3);
        }
        ub.cleanup();
        ctx.cleanup();
    }
    
}
