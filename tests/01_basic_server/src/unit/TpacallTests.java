import org.junit.Test;
import static org.junit.Assert.*;
import org.endurox.*;
import org.endurox.exceptions.AtmiTPEBADDESCException;
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
        
        int cd = ctx.tpacall("DROPRSP", null, 0);

        ctx.tpgetrply(cd, ub, 0);
        
        ub.cleanup();
        ctx.cleanup();
    }
    
    /**
     * Cross buffer type validation, async
     */
    @Test
    public void bufferCrossTestAsync() {
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        boolean leaktest = false;
        int leaktestSec = 0;
        StopWatch w = new StopWatch();
        
        Commons com = new Commons();
        
        String leaktestSecStr = System.getenv("NDRXJ_LEAKTEST");
        
        if (null!=leaktestSecStr)
        {
            leaktestSec = Integer.parseInt(leaktestSecStr);
            leaktest = true;
            
            //Nothing to test at the moment
            if (!System.getenv("NDRXJ_LEAKTEST_NAME").equals("bufferCrossTestAsync")) {
                return;
            }
        }
        
        for (int n=0; ((n<1000) || (leaktest && w.deltaSec() < leaktestSec)); n++) {
            
            com.bufferCrossTestX(ctx, true);
            
        }
    }
    
     /**
     * Cancel acalls
     */
    @Test
    public void tpcancelTest() {
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        boolean leaktest = false;
        int leaktestSec = 0;
        StopWatch w = new StopWatch();

        String leaktestSecStr = System.getenv("NDRXJ_LEAKTEST");
        
        if (null!=leaktestSecStr)
        {
            leaktestSec = Integer.parseInt(leaktestSecStr);
            leaktest = true;
            
            //Nothing to test at the moment
            if (!System.getenv("NDRXJ_LEAKTEST_NAME").equals("tpcancelTest")) {
                return;
            }
        }
        
        /* have some longer timeout */
        ctx.tptoutset(10);
        
        /* Overfill the max call descriptors, shall be no prob */
        for (int n=0; ((n<2000) || (leaktest && w.deltaSec() < leaktestSec)); n++) {
            
            int cd = ctx.tpacall("DROPRSP", null, 0);
            ctx.tpcancel(cd);
            
            boolean got_inval = false;
            
            try {
                TpgetrplyResult r = ctx.tpgetrply(cd, null, 0);
            } catch (AtmiTPEBADDESCException e) {
                got_inval = true;
            }
            
            assertEquals(true, got_inval);
        }
    }
    
}
