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
    int replies = 0;
    /**
     * Receive different types of messages
     * @param ctx Atmi context
     * @param buf typed buffer received
     * @param flags flags
     */
    public void unsolCallback(AtmiCtx ctx, TypedBuffer buf, long flags)
    {
        TpTypesResult t = null;
        
        if (null!=buf) {
            t = buf.tptypes();
        }
        
        ctx.tplogInfo("Got notif %b",buf);
        
        if (null==buf) {
            nrnull++;
            replies++;
        }
        else if (t.getType().equals("STRING")) {
            TypedString s = (TypedString)buf;
            /* Check the value sent in... */
            String ss = s.getString();
            assertEquals("HELLO NOTIF", ss);
            nrstring++;
            replies++;
        } 
        else if (t.getType().equals("JSON")) {
            TypedJson j = (TypedJson)buf;
            /* Check the value sent in... */
            String js = j.getJSON();
            assertEquals("{}", js);
            nrjson++;
            replies++;
        } 
        else if (t.getType().equals("CARRAY")) {
            TypedCarray c = (TypedCarray)buf;
            byte [] byt = c.getBytes();
            
            assertArrayEquals(new byte [] {0, 1, 2, 3, 4, 5, 6}, byt);
            nrcarray++;
            replies++;
        }
        else if (t.getType().equals("UBF")) {
            TypedUbf ub = (TypedUbf)buf;
            /* Check the value sent in... */
            String s = ub.BgetString(test.T_STRING_10_FLD, 5);
            assertEquals("HELLO UBF FROM SERVICE", s);
            nrubf++;
            replies++;
        }
        else if (t.getType().equals("VIEW")) {
            TypedView v = (TypedView)buf;
            assertEquals("JVIEW2", t.getSubType());
            nrview++;
            replies++;
        }
    }
    
    /**
     * Perform server call, this will wait for reply 
     * @param ctx
     * @param buf
     * @throws InterruptedException 
     */
    void doCall(AtmiCtx ctx, TypedBuffer buf) {
        
        int cur = replies;
        int sleeps = 0;
        ctx.tpcall("GLOBNOTIF", buf, 0);
        
        while (replies == cur && sleeps<4000) {
        
		/* ignore intr */
		try {
			Thread.sleep(50);
		} catch (InterruptedException e) {
		
		}
		
                sleeps++;
                
                //wait for message...
		ctx.tpchkunsol();
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
            replies = 0;
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
            doCall(ctx, null);
            assertEquals(1, nrnull);
            
            ctx.tplogInfo("*** STRING test ***");
            TypedString s = (TypedString)ctx.tpalloc("STRING", "", 1024);
            assertNotEquals(s, null);            
            doCall(ctx, s);
            assertEquals(1, nrstring);
            s.cleanup();
            
            ctx.tplogInfo("*** JSON test ***");
            TypedJson j = (TypedJson)ctx.tpalloc("JSON", "", 1024);
            assertNotEquals(j, null);            
            doCall(ctx, j);
            assertEquals(1, nrjson);
            j.cleanup();
            
            ctx.tplogInfo("*** CARRAY test ***");
            TypedCarray c = (TypedCarray)ctx.tpalloc("CARRAY", "", 1024);
            assertNotEquals(c, null);            
            doCall(ctx, c);
            assertEquals(1, nrcarray);
            c.cleanup();
            
            ctx.tplogInfo("*** VIEW test ***");
            TypedView v = (TypedView)ctx.tpalloc("VIEW", "JVIEW1", 1024);
            assertNotEquals(c, null);            
            doCall(ctx, v);
            assertEquals(1, nrview);
            v.cleanup();
            
            ctx.tplogInfo("*** UBF test ***");
            TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
            assertNotEquals(ub, null);            
            doCall(ctx, ub);
            assertEquals(1, nrubf);
            ub.cleanup();
            
        }

        ctx.cleanup();
    }
    
}
