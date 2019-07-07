import org.junit.Test;
import static org.junit.Assert.*;
import org.endurox.*;
import org.endurox.exceptions.UbfBNOTPRESException;

/**
 * Test server threads, with tpacall()
 */
public class SrvThreads {
    
    /**
     * Perform server threads test
     */
    @Test
    public void srvThreads() {
        
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
            if (!System.getenv("NDRXJ_LEAKTEST_NAME").equals("srvThreads")) {
                return;
            }
                
        }
        
        for (int i=0; ((i<300) || (leaktest && w.deltaSec() < leaktestSec)); i++)
        {
            try {
                ub.Bdelall(test.T_STRING_FLD);
            } 
            catch (UbfBNOTPRESException e)
            {
                // ignore.. 
            }
            
            ub.Bchg(test.T_STRING_FLD, 0, "UBF OK 1");

            
            ctx.tpacall("ThreadedSvc", ub, 0);
            ctx.tpacall("ThreadedSvc", ub, 0);
            ctx.tpacall("ThreadedSvc", ub, 0);
            
            /* get answers, 1 */
            try {
                ub.Bdelall(test.T_STRING_FLD);
            } 
            catch (UbfBNOTPRESException e)
            {
                // ignore.. 
            }
            
            ctx.tpgetrply(0, ub, AtmiConst.TPGETANY);
            String rspData = ub.BgetString(test.T_STRING_FLD, 1);
            assertEquals("UBF OK 2", rspData);
            
            /* get answers, 2 */
            try {
                ub.Bdelall(test.T_STRING_FLD);
            } 
            catch (UbfBNOTPRESException e)
            {
                // ignore.. 
            }
            
            ctx.tpgetrply(0, ub, AtmiConst.TPGETANY);
            rspData = ub.BgetString(test.T_STRING_FLD, 1);
            assertEquals("UBF OK 2", rspData);
            
            /* get answers, 3 */
            try {
                ub.Bdelall(test.T_STRING_FLD);
            } 
            catch (UbfBNOTPRESException e)
            {
                // ignore.. 
            }
            
            ctx.tpgetrply(0, ub, AtmiConst.TPGETANY);
            rspData = ub.BgetString(test.T_STRING_FLD, 1);
            assertEquals("UBF OK 2", rspData);            
            
        }
        ub.cleanup();
        ctx.cleanup();
    }
    
}
