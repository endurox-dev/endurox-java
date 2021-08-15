import static org.junit.Assert.*;
import org.endurox.*;
import org.endurox.exceptions.AtmiException;
import org.endurox.exceptions.AtmiTPEBLOCKException;
import org.junit.Test;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;


/**
 * Perform tpbroadcast tests
 * we will create another thread with this class 
 * Test works this way:
 * we start two threads where each waits for broadcasts.
 * One of the threads calls GLOBBROAD service, where in turn
 * service publishes 4x matched notifications. We wait for 200ms before
 * we run 
 */
public class TpBroadcastTests implements UnsolCallback, Runnable {
             
    int nrstring = 0 ;
    int nrjson = 0;
    int nrubf = 0 ;
    int nrcarray = 0;
    int nrnull = 0;
    int nrview = 0;
    int ncallbacks = 0;
    boolean running = true;
    private final Lock lock = new ReentrantLock(true);

    public void run() {
        
        //Create new ATMI Context...
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        ctx.tpsetunsol(this);
        
        //loop over the tpcheckunsol and test for responses....
        //and count the calls...
        
        while (running) {
            
            //Check for unsol messages...
            
            ctx.tpchkunsol();
            
            try {
                Thread.sleep(0, 50);
            } 
            catch (Exception e) {
                ctx.tplogex(AtmiConst.LOG_ERROR, "Failed to sleep", e);
            }
            
        }
        
        ctx.tplogError("Exitting...!");
    } 
    
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
            lock.lock();
            nrnull++;
            lock.unlock();
        }
        else if (t.getType().equals("STRING")) {
            TypedString s = (TypedString)buf;
            /* Check the value sent in... */
            String ss = s.getString();
            assertEquals("HELLO NOTIF", ss);
            lock.lock();
            nrstring++;
        } 
        else if (t.getType().equals("JSON")) {
            TypedJson j = (TypedJson)buf;
            /* Check the value sent in... */
            String js = j.getJSON();
            assertEquals("{}", js);
            lock.lock();
            nrjson++;
            lock.unlock();
        } 
        else if (t.getType().equals("CARRAY")) {
            TypedCarray c = (TypedCarray)buf;
            byte [] byt = c.getBytes();
            
            assertArrayEquals(new byte [] {0, 1, 2, 3, 4, 5, 6}, byt);
            lock.lock();
            nrcarray++;
            lock.unlock();
        }
        else if (t.getType().equals("UBF")) {
            TypedUbf ub = (TypedUbf)buf;
            /* Check the value sent in... */
            String s = ub.BgetString(test.T_STRING_10_FLD, 5);
            assertEquals("HELLO UBF FROM SERVICE", s);
            lock.lock();
            nrubf++;
            lock.unlock();
        }
        else if (t.getType().equals("VIEW")) {
            TypedView v = (TypedView)buf;
            assertEquals("JVIEW2", t.getSubType());
            lock.lock();
            nrview++;
            lock.unlock();
        }
        
        //So that above stats are filled.
        lock.lock();
        ncallbacks++;
        lock.unlock();
    }
    
    /**
     * Perform server call, this will wait for reply 
     * @param ctx
     * @param buf
     * @throws InterruptedException 
     */
    void doCall(AtmiCtx ctx, TypedBuffer buf, TpBroadcastTests other ) throws InterruptedException {
        
        int sleeps=0;
        int curcbs = ncallbacks;
        int curcbs_other = other.ncallbacks;
        int cd = ctx.tpacall("GLOBBROAD", buf, 0);

        Thread.sleep(50);
        ctx.tpgetrply(cd, null, 0);

        lock.lock();
        while ((ncallbacks != curcbs+4  || other.ncallbacks != curcbs_other+4) && sleeps<4000)
        {
            lock.unlock();

            try {
                ctx.tpgetrply(cd, null, AtmiConst.TPGETANY | AtmiConst.TPNOBLOCK);
            } catch (AtmiTPEBLOCKException e) {
                //sleep only if blocked...
                Thread.sleep(50);
                sleeps++;
            }
            lock.lock();
        }
        lock.unlock();
        //ctx.tpgetrply(cd, buf, 0);
    }
    
    @Test
    public void tpbroadcastTest() throws InterruptedException {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        
        boolean leaktest = false;
        int leaktestSec = 0;
        StopWatch w = new StopWatch();
        
        TpBroadcastTests other = new TpBroadcastTests();
        
        String leaktestSecStr = System.getenv("NDRXJ_LEAKTEST");
        
        if (null!=leaktestSecStr)
        {
            leaktestSec = Integer.parseInt(leaktestSecStr);
            leaktest = true;
            
            //Nothing to test at the moment
            if (!System.getenv("NDRXJ_LEAKTEST_NAME").equals("tpbroadcastTest")) {
                return;
            }
        }
        
        Thread otherTh = new Thread(other);
        
        ctx.tpsetunsol(this);
        
        otherTh.start();
        
        /* wait thread to start... */
        Thread.sleep(1000);
        
        /**
         * TODO: Have long term test for memory management.
         * ideally we would time terminated tests, for example 5 min...?
         * thus we need a stop watch construction to have in java..
         */
        for (int i=0; ((i<100) || (leaktest && w.deltaSec() < leaktestSec)); i++)
        {
                         
            int prev_nrnull;
            int prev_nrstring;
            int prev_nrjson;
            int prev_nrcarray;
            int prev_nrview;
            int prev_nrubf;

            lock.lock();
            prev_nrnull = nrnull;
            prev_nrstring = nrstring;
            prev_nrjson = nrjson;
            prev_nrcarray = nrcarray;
            prev_nrview = nrview;
            prev_nrubf = nrubf;
            lock.unlock();

            
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
            try {
                

                ctx.tplogInfo("*** NULL test *** ");
                doCall(ctx, null, other);

                lock.lock();
                try {
                    assertEquals(prev_nrnull + 4, nrnull);
                    assertEquals(other.nrnull, nrnull);
                } finally {
                    lock.unlock();
                }

                ctx.tplogInfo("*** STRING test ***");
                TypedString s = (TypedString)ctx.tpalloc("STRING", "", 1024);
                assertNotEquals(s, null);            
                doCall(ctx, s, other);

                lock.lock();
                try {
                    assertEquals(prev_nrstring + 4, nrstring);
                    assertEquals(other.nrstring, nrstring);
                } finally {
                    lock.unlock();
                }

                ctx.tplogInfo("*** JSON test ***");
                TypedJson j = (TypedJson)ctx.tpalloc("JSON", "", 1024);
                assertNotEquals(j, null);            
                doCall(ctx, j, other);

                lock.lock();
                try {
                    assertEquals(prev_nrjson+4, nrjson);
                    assertEquals(other.nrjson, nrjson);
                } finally {
                    lock.unlock();
                }

                ctx.tplogInfo("*** CARRAY test ***");
                TypedCarray c = (TypedCarray)ctx.tpalloc("CARRAY", "", 1024);
                assertNotEquals(c, null);            
                doCall(ctx, c, other);

                lock.lock();
                try {
                    assertEquals(prev_nrcarray + 4, nrcarray);
                    assertEquals(other.nrjson, nrcarray);
                } finally {
                    lock.unlock();
                }

                ctx.tplogInfo("*** VIEW test ***");
                TypedView v = (TypedView)ctx.tpalloc("VIEW", "JVIEW1", 1024);
                assertNotEquals(c, null);            
                doCall(ctx, v, other);

                lock.lock();
                try {
                    assertEquals(prev_nrview + 4, nrview);
                    assertEquals(other.nrview, nrview);
                } finally {
                    lock.unlock();
                }

                ctx.tplogInfo("*** UBF test ***");
                TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
                assertNotEquals(ub, null);            
                doCall(ctx, ub, other);

                lock.lock();
                try {
                    assertEquals(prev_nrubf + 4, nrubf);
                    assertEquals(other.nrubf, nrubf);
                } finally {
                    lock.unlock();
                }
            
            } 
            catch (AssertionError e) {
                other.running = false;
                Thread.sleep(1000, 0);
                throw e;
            }
            
        }

        //Wait for other thread to finish...
        other.running = false;
        Thread.sleep(1000, 0);
                
        ctx.cleanup();
    }
    
}
