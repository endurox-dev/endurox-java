import org.junit.Test;
import static org.junit.Assert.*;
import org.endurox.*;
import org.endurox.exceptions.UbfBNOTPRESException;

/**
 * Queue tests
 */
public class QueueTests {
             
    /**
     * Perform tpcall test
     */
    @Test
    public void basicQTest() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);;
        
        boolean leaktest = false;
        int leaktestSec = 0;
        StopWatch w = new StopWatch();
        
        String leaktestSecStr = System.getenv("NDRXJ_LEAKTEST");
        
        if (null!=leaktestSecStr)
        {
            leaktestSec = Integer.parseInt(leaktestSecStr);
            leaktest = true;
            
            //Nothing to test at the moment
            if (!System.getenv("NDRXJ_LEAKTEST_NAME").equals("basicQTest")) {
                return;
            }
                
        }
        
        Commons com = new Commons();
        
        int curTest = 4391;
        /**
         * TODO: Have long term test for memory management.
         * ideally we would time terminated tests, for example 5 min...?
         * thus we need a stop watch construction to have in java..
         */
        for (int i=0; ((i<1000) || (leaktest && w.deltaSec() < leaktestSec)); i++)
        {
            
            /* For types */
            String [] buffers = new String[] {"NULL", "STRING", "JSON", "VIEW", "UBF", "CARRAY"};
            
            for (int j=0; j<buffers.length; j++) {
                
                String isub = "";
                byte [] corrid = new byte[AtmiConst.TMCORRIDLEN];
                if (buffers[j].equals("VIEW")) {
                    isub = "JVIEW1";
                }

                curTest++;
                int deqTestId1 = curTest;
                
                /* Alloc value, enqueue with with out id */
                ctx.tplogInfo("Test id 1: %d", curTest);
                TypedBuffer b = com.getTestBuffer(ctx, buffers[j], isub, curTest);
                TPQCTL ctl = new TPQCTL();
                ctx.tpenqueue("MYSPACE", "TESTQ", ctl, b, 0);
                
                /* Alloc value, enqueue with with id */
                curTest++;
                int deqTestId2 = curTest;
                ctx.tplogInfo("Test id 2: %d", curTest);
                b = com.getTestBuffer(ctx, buffers[j], isub, curTest);
                ctl = new TPQCTL();
                
                corrid[0] = (byte)(curTest >> 24);
                corrid[1] = (byte)(curTest >> 16);
                corrid[2] = (byte)(curTest >> 8);
                corrid[3] = (byte)(curTest);
                
                ctx.tplogInfo("corr id %d = %d %d %d %d %d", curTest, corrid[0], corrid[1], 
                        corrid[2], corrid[3], corrid[4]);
                
                ctl.setCorrid(corrid);
                ctl.setFlags(AtmiConst.TPQCORRID);
                ctx.tpenqueue("MYSPACE", "TESTQ", ctl, b, 0);
                
                /* Alloc value, enqueue with with out id */
                curTest++;
                int deqTestId3 = curTest;
                
                ctx.tplogInfo("Test id 3: %d", curTest);
                b = com.getTestBuffer(ctx, buffers[j], isub, curTest);
                ctl = new TPQCTL();
                ctx.tpenqueue("MYSPACE", "TESTQ", ctl, b, 0);
                
                /* try to read by msg id... */
                ctl = new TPQCTL();
                
                /* set corr & test id we expected...*/
                curTest = deqTestId2;
                ctx.tplogInfo("dequeue corr id %d = %d %d %d %d %d", 
                        curTest, corrid[0], corrid[1],  corrid[2], 
                        corrid[3], corrid[4]);
                ctl.setCorrid(corrid);
                ctl.setFlags(AtmiConst.TPQGETBYCORRID);
                TypedBuffer qmsg = ctx.tpdequeue("MYSPACE", "TESTQ", ctl, b, 0);
                
                com.testBuffer(ctx, buffers[j], isub, qmsg, curTest);
                
                /* dequeue with out id, 1 */
                ctl = new TPQCTL();
                qmsg = ctx.tpdequeue("MYSPACE", "TESTQ", ctl, null, 0);
                com.testBuffer(ctx, buffers[j], isub, qmsg, deqTestId1);
                
                /* dequeue with out id, 3 */
                ctl = new TPQCTL();
                qmsg = ctx.tpdequeue("MYSPACE", "TESTQ", ctl, null, 0);
                com.testBuffer(ctx, buffers[j], isub, qmsg, deqTestId3);
                
                
            }
                
                /* Alloc value, enqueue with with out id */
                
                /* dequeue with id */
                /* dequeue with id, no id */
                /* dequeue with id, no id */
                /* dequeue with id, no id => no msg exception..*/
                /* dequeue invalid queue => no msg exception..*/

        }
        ctx.cleanup();
    }    
    /* TODO: Test queue with different buffer kinds... */
}
