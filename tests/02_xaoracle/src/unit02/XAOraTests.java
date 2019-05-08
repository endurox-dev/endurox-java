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
        
        ctx.tpopen();
        ctx.tpbegin(60, 0);
        
        /* create some test table */
        conn = ctx.getConnection();
        
        assertNotEquals(null, conn);
        
        try {
            conn.setAutoCommit(false);
            stmt = conn.createStatement();
        } 
        catch (Exception e) {
            /* TODO: LOG Error + exception */
            ctx.tplogex(AtmiConst.LOG_ERROR, "Failed to create statement", e);
            assertNull(e);
        }    
        
        String sql = "DROP TABLE EXJTEST";
        try {
            conn.setAutoCommit(false);
            stmt.executeUpdate(sql);
        }
        catch (Exception e) {
            ctx.tplogex(AtmiConst.LOG_DEBUG, "Failed to drop EXJTEST", e);
        }
        
        /* create table */
        
        sql = "CREATE TABLE EXJTEST\n" +
            "( customer_id number(10) NOT NULL,\n" +
            "  customer_name varchar2(50) NOT NULL,\n" +
            "  city varchar2(50)\n" +
            ");";
        
        try {
            stmt.executeUpdate(sql);
        }
        catch (Exception e) {
            ctx.tplogex(AtmiConst.LOG_DEBUG, "Failed to drop EXJTEST", e);
        }
        
        ctx.tpcommit(0);
        ctx.tpclose();
        
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
            
            ctx.tpopen();
            
            /* run the transaction */
            ctx.tpbegin(60, 0);
            
            ctx.tpcommit(0);
            
            ctx.tpclose();
            
        }
        
        ub.cleanup();
        ctx.cleanup();
    }
    
}
