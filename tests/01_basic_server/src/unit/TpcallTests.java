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
    
    //TODO: Test buffer with other service error, for example noent...
    
    //TODO: Test server call with buffer translate...
    
    //TODO: Test server call with buffer translate and service error...
    
    //TODO: have a test for message forwarding...
    
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
     * Cross validate 
     * @param svc Service name
     * @param input_type intput buffer type
     * @param input_sub input sub-buffer type
     * @param output_type output buffer type
     * @param output_sub output buffer sub-type
     * @param flags call flags
     * @param tpcallerr return error code/from exception
     * @param validate shall we validate the error
     * @return SUCCEED/FAIL
     */
    int testerX(AtmiCtx ctx, String svc, String input_type, String input_sub, 
        String output_type, String output_sub, long flags, int tpcallerr, boolean validate) {
        
        int ret = AtmiConst.SUCCEED;
        TypedBuffer b = null;
        TypedBuffer b_ret;
        int excpt_err = 0;
        
        ctx.tplogInfo("svc: [%s] input_type: [%s] input_sub: [%s] output_type: "+
            "[%s] output_sub: [%s] flags: %ld tpcallerr: %d validate: %b",
            svc, input_type, input_sub, output_type, output_sub, flags, 
            tpcallerr, validate);
        
        if (input_type.equals("NULL")) {
            b = null;
        } 
        else if (input_type.equals("STRING")) {
            b = ctx.newString("This is unit test");
        }
        else if (input_type.equals("JSON")) {
            b = ctx.newJson("[]");
        }
        else if (input_type.equals("CARRAY")) {
            b = ctx.newCarray(new byte [] {9, 4, 3, 2});
        }
        else if (input_type.equals("VIEW")) {
            b = ctx.tpalloc(input_type, input_sub, 0);
        }
        else if (input_type.equals("UBF")) {
            TypedUbf u;
            b = ctx.tpalloc(input_type, "", 0);
            
            u = (TypedUbf)b;   
            u.Bchg(test.T_FLOAT_2_FLD, 11, 1.11111d);
        }
        
        /* call the service */
        
        try {
            
            b_ret = ctx.tpcall(svc, b, flags);
            
        } catch (AtmiException e) {
            
            ctx.tplogex(AtmiConst.LOG_ERROR, "Got exception", e);
            excpt_err = e.getErrorCode();
            
            if (null!=e.getReplyBuffer())
            {
                b_ret = e.getReplyBuffer();
            } else {
                b_ret = b;
            }
        }
        
        /* validate the response */
        assertEquals(tpcallerr, excpt_err);
        
        /* validate data types... */
            
        out:
        return ret;
        
    }
    
}
