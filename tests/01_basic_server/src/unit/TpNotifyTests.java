import static org.junit.Assert.*;
import org.endurox.*;
import org.endurox.exceptions.AtmiException;
import org.endurox.exceptions.UbfBNOTPRESException;
import org.junit.Test;

/**
 * Perform tpnotify tests
 */
public class TpNotifyTests implements UnsolCallback {
             
    int nrstrings = 0 ;
    int nrjson = 0;
    int nrubf = 0 ;
    int nrcarrays = 0;
    int nrnull = 0;
    int nrview = 0;
    
    /**
     * Receive different types of messages
     * @param ctx Atmi context
     * @param buf typed buffer received
     * @param flags flags
     */
    public void unsolCallback(AtmiCtx ctx, TypedBuffer buf, long flags)
    {
        TpTypesResult t = buf.tptypes();
        
        if (t.getType().equals("STRING")) {
            nrstrings++;
        } else if (t.getType().equals("JSON")) {
            nrjson++;
        } else if (t.getType().equals("UBF")) {
            TypedUbf ub = (TypedUbf)buf;
            nrubf++;
            
            /* Check the value sent in... */
            String s = ub.BgetString(test.T_STRING_10_FLD, 5);
            assertEquals("HELLO UBF FROM SERVICE", s);
            
        }
    }
    
    @Test
    public void tpnotifyTest() {
        
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
            if (!System.getenv("NDRXJ_LEAKTEST_NAME").equals("tpnotifyTest")) {
                return;
            }
        }
        
        ctx.tpsetunsol(this);
        
        /**
         * TODO: Have long term test for memory management.
         * ideally we would time terminated tests, for example 5 min...?
         * thus we need a stop watch construction to have in java..
         */
        for (int i=0; ((i<1000) || (leaktest && w.deltaSec() < leaktestSec)); i++)
        {
                         
            nrstrings = 0 ;
            nrjson = 0;
            nrubf = 0 ;
            nrcarrays = 0;
            nrnull = 0;
            nrview = 0;
            
            /* loop over the buffer types
             * send them to server and expect one to be received back..
             * each one we shall test with:
             * tpcall
             * tpgetrply
             * tpcheckunsol()
             */
            String reqData = String.format("loop %d", i);
            ub.Bchg(test.T_STRING_FLD, 0, String.format("loop %d", i));
            
            ub = (TypedUbf)ctx.tpcall("GLOBNOTIF", ub, 0);
            
            assertEquals(1, nrubf);
        }
        ub.cleanup();
        ctx.cleanup();
    }
    
}
