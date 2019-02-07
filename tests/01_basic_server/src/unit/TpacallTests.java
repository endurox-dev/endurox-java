import org.junit.Test;
import static org.junit.Assert.*;
import org.endurox.*;
import org.endurox.exceptions.UbfBNOTPRESException;
import org.endurox.exceptions.AtmiTPEINVALException;
import org.endurox.exceptions.AtmiTPETIMEException;

/**
 * Async tpcall tests
 */
public class TpacallTests {
             
    /**
     * Perform tpacall test
     */
    @Test
    public void tpacallTest() {
        
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
            if (!System.getenv("NDRXJ_LEAKTEST_NAME").equals("tpacallTest")) {
                return;
            }
            
        }
        
        /* get env for leak test... for particular time... */

        for (int i=0; ((i<1000) || (leaktest && w.deltaSec() < leaktestSec)); i++)
        {
            try {
                ub.Bdel(test.T_STRING_2_FLD, 0);
            } 
            catch (UbfBNOTPRESException e)
            {
                /* ignore.. */
            }
            
            String reqData = String.format("loop %d", i);
            ub.Bchg(test.T_STRING_FLD, 0, String.format("loop %d", i));
            
            int cd = ctx.tpacall("ECHOSVC", ub, 0);
            
            ctx.tplogInfo("Got cd = %d", cd);
            
            //Wait for reply...
            
            TpgetrplyResult res = ctx.tpgetrply(cd, ub, 0);
            
            //Extract the reply buffer.
            ub = (TypedUbf)res.getBuffer();
            
            String rspData = ub.BgetString(test.T_STRING_2_FLD, 0);
            
            assertEquals(reqData, rspData);
        }
        ub.cleanup();
        ctx.cleanup();
    }
    
    /**
     * Invalid cd usage...
     * Expect particular exception
     */
    @Test(expected = AtmiTPEINVALException.class)
    public void tpgetrplyInvalCdTest() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);

        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);

        ctx.tpgetrply(-1, ub, 0);
        
        ub.cleanup();
        ctx.cleanup();
    }
    
    /**
     * Timeout...
     */
    @Test(expected = AtmiTPETIMEException.class)
    public void tpgetrplyTimeoutTest() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);

        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);
        
        /* init client before setting tout...! */
        
        ctx.tpinit(null);
        
        ctx.tptoutset(1);

        ctx.tpgetrply(55, ub, 0);
        
        ub.cleanup();
        ctx.cleanup();
    }
    
}
