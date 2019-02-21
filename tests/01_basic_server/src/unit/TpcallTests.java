import org.junit.Test;
import static org.junit.Assert.*;
import org.endurox.*;
import org.endurox.exceptions.AtmiException;
import org.endurox.exceptions.UbfBNOTPRESException;

/**
 * Buffer field delete tests
 */
public class TpcallTests {
             
    /**
     * Perform tpcall test
     */
    @Test
    public void tpcallTest() {
        
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
            if (!System.getenv("NDRXJ_LEAKTEST_NAME").equals("tpcallTest")) {
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
            
            try {
                ub.Bdel(test.T_STRING_2_FLD, 0);
            } 
            catch (UbfBNOTPRESException e)
            {
                // ignore.. 
            }
            
            String reqData = String.format("loop %d", i);
            ub.Bchg(test.T_STRING_FLD, 0, String.format("loop %d", i));
            
            ub = (TypedUbf)ctx.tpcall("ECHOSVC", ub, 0);
            
            String rspData = ub.BgetString(test.T_STRING_2_FLD, 0);
            assertEquals(reqData, rspData);
        }
        ub.cleanup();
        ctx.cleanup();
    }
    
    /**
     * Test with NULL buffer call.... + non null response...
     */
    @Test
    public void tpcallNullCallTest() {
        
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
            if (!System.getenv("NDRXJ_LEAKTEST_NAME").equals("tpcallNullCallTest")) {
                return;
            }
                
        }
        
        for (int i=0; ((i<1000) || (leaktest && w.deltaSec() < leaktestSec)); i++)
        {
            try {
                ub.Bdel(test.T_STRING_2_FLD, 0);
            } 
            catch (UbfBNOTPRESException e)
            {
                // ignore.. 
            }
            
            ub = (TypedUbf)ctx.tpcall("NULL", null, 0);
            
            String rspData = ub.BgetString(test.T_STRING_2_FLD, 0);
            assertEquals("HELLO NULL", rspData);
        }
        ub.cleanup();
        ctx.cleanup();
    }
    
    /**
     * Send NULL and get NULL
     */
    @Test
    public void tpcallNullNull() {
        
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
            if (!System.getenv("NDRXJ_LEAKTEST_NAME").equals("tpcallNullNull")) {
                return;
            }
        }
        
        for (int i=0; ((i<1000) || (leaktest && w.deltaSec() < leaktestSec)); i++)
        {
            TypedUbf ub = null;
        
            ub = (TypedUbf)ctx.tpcall("NULLRSP", null, 0);
            
            assertEquals(null, ub);
        }
        ctx.cleanup();
    }
    
    /**
     * We send some data but we get NULL back...
     */
    @Test
    public void tpcallNullRsp() {
        
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
            if (!System.getenv("NDRXJ_LEAKTEST_NAME").equals("tpcallNullRsp")) {
                return;
            }
                
        }
        
        for (int i=0; ((i<1000) || (leaktest && w.deltaSec() < leaktestSec)); i++)
        {
            TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
            assertNotEquals(ub, null);
            
            /* load some data */
            
            ub.Badd(test.T_STRING_2_FLD, "HELLO NULL");
        
            ub = (TypedUbf)ctx.tpcall("NULLRSP", ub, 0);
            
            assertEquals(null, ub);
        }
        ctx.cleanup();
    }
    
    /**
     * Cross buffer type validation
     */
    @Test
    public void bufferCrossTest() {
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
            if (!System.getenv("NDRXJ_LEAKTEST_NAME").equals("bufferCrossTest")) {
                return;
            }
        }
        
        for (int n=0; ((n<1000) || (leaktest && w.deltaSec() < leaktestSec)); n++) {
            
            com.bufferCrossTestX(ctx, false);
            
        }
    }
    
}
