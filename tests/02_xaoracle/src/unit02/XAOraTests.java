import java.sql.Connection;
import java.sql.Statement;
import org.junit.Test;
import static org.junit.Assert.*;
import org.endurox.*;

/**
 * OracleDB XA Tests
 */
public class XAOraTests {
             
    /**
     * Perform Basic XA Tests
     */
    @Test
    public void basicXA() {
        
        Connection conn = null;
        Statement stmt = null;
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
            if (!System.getenv("NDRXJ_LEAKTEST_NAME").equals("basicXA")) {
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
            /* TODO: Do the logic */
            ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
            assertNotEquals(ub, null);
            
            ctx.tpopen();
            /* create some test table */
            conn = ctx.getConnection();
            assertNotEquals(null, conn);

            /* run the transaction */
            ctx.tpbegin(60, 0);
            
            /* call the server, insert some data */
            
            ub.Bchg(test.T_LONG_FLD, 0, (long)i);
            ub.Bchg(test.T_STRING_FLD, 0, String.format("Name %d", i));
            ub.Bchg(test.T_STRING_2_FLD, 0, String.format("City %d", i));
            
            /* well we shall suspend our side here... */
            ub = (TypedUbf)ctx.tpcall("DoTran", ub, AtmiConst.TPTRANSUSPEND);

            ctx.tpcommit(0);
            ctx.tpclose();
            
        }
        
        ub.cleanup();
        ctx.cleanup();
    }
    
}
