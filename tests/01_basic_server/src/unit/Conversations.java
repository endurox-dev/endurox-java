import org.junit.Test;
import static org.junit.Assert.*;
import org.endurox.*;
import org.endurox.exceptions.AtmiException;
import org.endurox.exceptions.UbfBNOTPRESException;

/**
 * Conversations tests
 */
public class Conversations {
             
    /**
     * Perform conversations tests, with UBF buffer..
     */
    @Test
    public void convTest() {
        
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
            if (!System.getenv("NDRXJ_LEAKTEST_NAME").equals("convTest")) {
                return;
            }
                
        }
        
        /**
         * ideally we would time terminated tests, for example 5 min...?
         * thus we need a stop watch construction to have in java..
         */
        for (int i=0; ((i<1000) || (leaktest && w.deltaSec() < leaktestSec)); i++)
        {
            
            int cd, rcv;
            
            /* connect to server */
            cd = ctx.tpconnect("UBFCONV", ub, 0);
            
            /* receive until eof (should be 100) */
            rcv = 0;
            
            while (true)
            {
                rcv++;
                
                TprecvResult rec = ctx.tprecv(cd, ub, 0);
                
                ub = (TypedUbf)rec.getBuffer();
                
                if (rec.getRevent() != 0)
                {
                    assertEquals(AtmiConst.TPEV_SENDONLY, rec.getRevent());
                    break;
                }
            }
            
            assertEquals(100, rcv);
            
            /* send 100 */
            
            /* disconnect... */
            
            
            /* we should work out the unit test for different buffer types...
             * call with different buffer types, and expect some errors
             * we should do the same way as we did for tpcall/acall
             */
                        
        }
        ub.cleanup();
        ctx.cleanup();
    }
    
}
