import org.junit.Test;
import static org.junit.Assert.*;
import org.endurox.*;
import org.endurox.exceptions.UbfBNOTPRESException;

/**
 * Test client threads processing
 */
public class CltThreads {
             
    
    class ProcThread implements Runnable {
        
        private Thread t;
        private String threadName;

        boolean leaktest;
        int leaktestSec;
        int threadNr;
        int callsDone = 0;
        
        AtmiCtx ctx = new AtmiCtx();

        ProcThread(boolean leaktest, int leaktestSec, int threadNr) {
            this.leaktest = leaktest;
            this.leaktestSec = leaktestSec;
            this.threadNr = threadNr;
        }
        
        /**
         * Run the threaded test...
         */
        public void run() {
            
            StopWatch w = new StopWatch();
            Commons com = new Commons();
            /**
             * ideally we would time terminated tests, for example 5 min...?
             * thus we need a stop watch construction to have in java..
             */
            for (int i=0; ((i<100) || (leaktest && w.deltaSec() < leaktestSec)); i++)
            {
                com.bufferCrossTestX(ctx, false);
                callsDone++;
            }
            
        }
        
        /**
         * Run off the thread
         */
        public void start () {
            String name = Integer.toString(threadNr); 
            System.out.println(String.format("Starting thread Nr: %d", threadNr));
            if (t == null) {
                t = new Thread (this, name);
                t.start ();
            }
        }
        
        /**
         * Wait for complete..
         */
        public void join() throws InterruptedException{
            t.join();
        }
        
        public int done() {
            return callsDone;
        }
        
    }
    
    /**
     * Perform test of tpforward
     */
    @Test
    public void clThreadsTest() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        
        boolean leaktest = false;
        int leaktestSec = 0;
        StopWatch w = new StopWatch();
        ProcThread [] threads = new ProcThread[5];
        int i;
        
        String leaktestSecStr = System.getenv("NDRXJ_LEAKTEST");
        
        if (null!=leaktestSecStr)
        {
            leaktestSec = Integer.parseInt(leaktestSecStr);
            leaktest = true;
            
            //Nothing to test at the moment
            if (!System.getenv("NDRXJ_LEAKTEST_NAME").equals("clThreadsTest")) {
                return;
            }
                
        }
        
        /* todo: call threads & join them... */
        for (i=0; i<threads.length; i++)
        {
            threads[i] = new ProcThread(leaktest, leaktestSec, i);
            threads[i].start();
        }
        
        for (i=0; i<threads.length; i++)
        {
            ctx.tplogInfo("Waiting on thread: %d", i);
            try {
                threads[i].join();
                assertNotEquals(threads[i].done(), 0);
            } catch (Exception e) {
                
                 assertTrue(String.format("Exception on thread: %d: %s", i, e.toString()), 
                         false);
            }
        }
        
        ctx.cleanup();
    }
    
}
