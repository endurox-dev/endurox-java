import static org.junit.Assert.*;
import org.endurox.*;
import org.endurox.exceptions.AtmiException;
import org.endurox.exceptions.UbfBNOTPRESException;
import org.junit.Test;

/**
 * Perform tpnotify tests
 */
public class TpNotifyTests implements UnsolCallback {
             
    int nrstring = 0 ;
    int nrjson = 0;
    int nrubf = 0 ;
    int nrcarray = 0;
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
            TypedString s = (TypedString)buf;
            /* Check the value sent in... */
            String ss = s.getString();
            assertEquals("HELLO NOTIF", ss);
            nrstring++;
        } else if (t.getType().equals("JSON")) {
            TypedJson j = (TypedJson)buf;
            /* Check the value sent in... */
            String js = j.getJSON();
            assertEquals("{}", js);
            nrjson++;
        } else if (t.getType().equals("UBF")) {
            TypedUbf ub = (TypedUbf)buf;
            /* Check the value sent in... */
            String s = ub.BgetString(test.T_STRING_10_FLD, 5);
            assertEquals("HELLO UBF FROM SERVICE", s);
            nrubf++;
        }
    }
    
    @Test
    public void tpnotifyTest() {
        
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
                         
            nrnull = 0;
            nrstring = 0 ;
            nrjson = 0;
            nrcarray = 0;
            nrview = 0;
            nrubf = 0 ;
            /* loop over the buffer types
             * send them to server and expect one to be received back..
             * each one we shall test with:
             * tpcall
             * tpgetrply
             * tpcheckunsol()
             * In case of broadcast we can test following:
             * - test deliver to this and other thread (our binary name), matched
             * - test binary name not matched (0 bordcasts)
             * - test binary named matched by regexp (i.e. flags test).
             */

            ctx.tplogInfo("*** NULL test *** ");
            ctx.tpcall("GLOBNOTIF", null, 0);
            assertEquals(1, nrnull);
            
            ctx.tplogInfo("*** STRING test ***");
            TypedString s = (TypedString)ctx.tpalloc("STRING", "", 1024);
            assertNotEquals(s, null);            
            ctx.tpcall("GLOBNOTIF", s, 0);
            assertEquals(1, nrstring);
            
            ctx.tplogInfo("*** JSON test ***");
            TypedJson j = (TypedJson)ctx.tpalloc("JSON", "", 1024);
            assertNotEquals(j, null);            
            ctx.tpcall("GLOBNOTIF", j, 0);
            assertEquals(1, nrjson);
            
            ctx.tplogInfo("*** CARRAY test ***");
            TypedCarray c = (TypedCarray)ctx.tpalloc("CARRAY", "", 1024);
            assertNotEquals(c, null);            
            ctx.tpcall("GLOBNOTIF", c, 0);
            assertEquals(1, nrcarray);
            
            ctx.tplogInfo("*** VIEW test ***");
            TypedView v = (TypedView)ctx.tpalloc("VIEW", "JVIEW1", 1024);
            assertNotEquals(c, null);            
            ctx.tpcall("GLOBNOTIF", v, 0);
            assertEquals(1, nrview);
            
            ctx.tplogInfo("*** UBF test ***");
            TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
            assertNotEquals(ub, null);            
            ctx.tpcall("GLOBNOTIF", ub, 0);
            assertEquals(1, nrubf);
            
        }

        ctx.cleanup();
    }
    
}
