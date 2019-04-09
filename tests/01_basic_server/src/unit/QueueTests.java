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
        
        /**
         * TODO: Have long term test for memory management.
         * ideally we would time terminated tests, for example 5 min...?
         * thus we need a stop watch construction to have in java..
         */
        for (int i=0; ((i<1000) || (leaktest && w.deltaSec() < leaktestSec)); i++)
        {
            /*
            TPQCTL ctl = new TPQCTL();
            
            try {
                ub.Bdel(test.T_STRING_2_FLD, 0);
            } 
            catch (UbfBNOTPRESException e)
            {
                // ignore.. 
            }
            
            String reqData = String.format("loop %d", i);
            ub.Bchg(test.T_STRING_FLD, 0, String.format("loop %d", i));
            
            ctx.tpenqueue("MYSPACE", reqData, ctl, ub, 0);
            
            
            ctl = new TPQCTL();
            TypedUbf retBuf = (TypedUbf)ctx.tpdequeue("MYSPACE", reqData, ctl, null, 0);
            
            String rspData = retBuf.BgetString(test.T_STRING_FLD, 0);
            assertEquals(String.format("loop %d", i), rspData);
            
            */
            
            /* For types */
            String [] buffers = new String[] {"NULL", "STRING", "JSON", "VIEW", "UBF", "CARRAY"};
            
            for (int j=0; j<buffers.length; j++) {

                String isub = "";
                byte [] corrid = new byte[AtmiConst.TMCORRIDLEN];
                if (buffers[j].equals("VIEW")) {
                    isub = "JVIEW1";
                }
                
                int testid = (j<<27 | i)<<2;
                int curTest;
                
                
                /* Alloc value, enqueue with with out id */
                curTest = testid+1;
                ctx.tplogInfo("Test id 1: %d", curTest);
                TypedBuffer b = com.getTestBuffer(ctx, buffers[j], isub, curTest);
                TPQCTL ctl = new TPQCTL();
                ctx.tpenqueue("MYSPACE", "TESTQ", ctl, b, 0);
                
                /* Alloc value, enqueue with with id */
                curTest = testid+2;
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
                curTest = testid+3;
                ctx.tplogInfo("Test id 3: %d", curTest);
                b = com.getTestBuffer(ctx, buffers[j], isub, curTest);
                ctl = new TPQCTL();
                ctx.tpenqueue("MYSPACE", "TESTQ", ctl, b, 0);
                
                /* try to read by msg id... */
                ctl = new TPQCTL();
                
                /* set corr */
                ctx.tplogInfo("dequeue corr id %d = %d %d %d %d %d", 
                        curTest, corrid[0], corrid[1],  corrid[2], 
                        corrid[3], corrid[4]);
                ctl.setCorrid(corrid);
                ctl.setFlags(AtmiConst.TPQCORRID);
                TypedBuffer qmsg = ctx.tpdequeue("MYSPACE", "TESTQ", ctl, b, 0);
                
                com.testBuffer(ctx, buffers[j], isub, qmsg, testid+2);
                
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
