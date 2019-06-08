import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import org.junit.Test;
import static org.junit.Assert.*;
import org.endurox.*;

/**
 * PostgresqlDB XA Tests
 */
public class XAPgTests implements  Runnable {
    
    
    /**
     * Is there shutdown request
     */
    public static boolean shutdownReq = false;
    
    Connection conn = null;
    
    public static final int TEST_MAX = 10;
    
    
    public void run() {
        System.out.print("Got shutdown request...\n");
        shutdownReq = true;
    }
    
    /**
     * Delete all records from db
     * TODO: try xid 64?
     */
    public void deleteAll(AtmiCtx ctx) {
        
        boolean tranStarted = false;
        Statement stmt = null;
        /*
        if (ctx.tpgetlev() == 0) {
            
            tranStarted = true;
            ctx.tpbegin(60, 0);
            ctx.tpcommit(0);
            
            ctx.tpbegin(60, 0);
            ctx.tpcommit(0);    
        }
        */
        tranStarted = true;
        ctx.tpbegin(60, 0);
        /* delete all if error then abort... */
        String sql = "delete from EXJTEST";
        
        try {
          stmt = conn.createStatement();
          
          stmt.executeUpdate(sql);
          System.out.println("Record deleted successfully");
          
        } catch (SQLException e) {
          ctx.tplogex(AtmiConst.LOG_ERROR, "Failed to delete: ".concat(sql), e);
        
          if (tranStarted) {
              ctx.tpabort(0);
          }
          
          throw new RuntimeException(e);
        }
            
        if (tranStarted) {
            try {
                
            } 
            catch (Exception e) {
                
                ctx.tplogex(AtmiConst.LOG_ERROR, "Failed to get getTransactionTimeout", e);
                
            }
            ctx.tpcommit(0);
        }
        
        try {
            stmt.close();
        } catch (SQLException e) {

            ctx.tplogex(AtmiConst.LOG_ERROR, "Failed to close stmt", e);
            throw new RuntimeException(e);
        }
        
    }
    
    /**
     * Check count in database
     * @param ctx Atmi Context
     * @param match number of records to match
     */
    public void chkCount(AtmiCtx ctx, int match) {
        
        boolean tranStarted = false;
        
        if (ctx.tpgetlev() == 0) {
            
            tranStarted = true;
            ctx.tpbegin(60, 0);
        }
        
        /* delete all if error then abort... */
        String sql = "select count(*) as count from EXJTEST";
        
        try {
            
            Statement stmt = conn.createStatement();
            ResultSet rs3 = stmt.executeQuery(sql);
            
            rs3.next();

            int count = rs3.getInt("count");
            
            assertEquals(match, count);
            stmt.close();
            
        } catch (SQLException e) {
            
            ctx.tplogex(AtmiConst.LOG_ERROR, "Failed to get count: ".concat(sql), e);

            if (tranStarted) {
                ctx.tpabort(0);
            }

            throw new RuntimeException(e);
        }
        
        if (tranStarted) {
            ctx.tpcommit(0);
        }
    }
             
    /**
     * Perform Basic XA Tests
     */
    @Test
    public void basicXA() {
        
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
       
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);

        boolean leaktest = false;
        int leaktestSec = 0;
        StopWatch w = new StopWatch();
        
        /**
         * Register shutdown hook
         
        Runtime.getRuntime().addShutdownHook(new Thread() {
            public void run() {
                System.console().printf("Shutdown requested...\n");
                XAPgTests.shutdownReq = true;
                
                while (!XAPgTests.shutdownDone)
                {
                    try {
                        System.console().printf("Shutdown wait...\n");
                        Thread.sleep(1000);
                    }
                    catch (Exception e) {
                        
                    }
                }
            }
        });
        * */
        
        ctx.installTermSigHandler(this);
        
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
        
        ctx.tpopen();
        
        try {
            conn = ctx.getConnection();
        }  catch (SQLException e) {
            ctx.tplogex(AtmiConst.LOG_ERROR, "Failed to get conn", e);
            assertEquals(null, e);
        }
        
        assertNotNull(conn);
        
        /**
         * TODO: Have long term test for memory management.
         * ideally we would time terminated tests, for example 5 min...?
         * thus we need a stop watch construction to have in java..
         */
        for (int i=0; ((i<10000) || (leaktest && w.deltaSec() < leaktestSec)); i++)
        {
            /* TODO: Do the logic */
            ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
            assertNotEquals(ub, null);
            
            
            /* create some test table 
            assertNotEquals(null, conn);
            */
            
            /* empty up the table... */
            deleteAll(ctx);
            
            /* run the transaction */
            ctx.tpbegin(160, 0);
            assertEquals(1, ctx.tpgetlev());

            for (int j=0; j<TEST_MAX; j++)
            {
                /* call the server, insert some data */
                ub.Bchg(test.T_LONG_FLD, 0, (long)j);
                ub.Bchg(test.T_STRING_FLD, 0, String.format("Name %d", j));
                ub.Bchg(test.T_STRING_2_FLD, 0, String.format("City %d", j));

                /* well we shall suspend our side here... */
                ub = (TypedUbf)ctx.tpcall("DoTran", ub, AtmiConst.TPTRANSUSPEND);
            }
            
            /* as we resume tran, get the count... */
            chkCount(ctx, TEST_MAX);
            
            /* lets abort */
            ctx.tpabort(0);
            
            /* there should be 0 after abort */
            chkCount(ctx, 0);
            
            
            /* todo perform abort */
            
            ctx.tpbegin(160, 0);
            assertEquals(1, ctx.tpgetlev());

            for (int j=0; j<TEST_MAX; j++)
            {
                /* call the server, insert some data */
                ub.Bchg(test.T_LONG_FLD, 0, (long)j);
                ub.Bchg(test.T_STRING_FLD, 0, String.format("Name %d", j));
                ub.Bchg(test.T_STRING_2_FLD, 0, String.format("City %d", j));

                /* well we shall suspend our side here... */
                ub = (TypedUbf)ctx.tpcall("DoTran", ub, AtmiConst.TPTRANSUSPEND);
            }
            
            /* as we resume tran, get the count... */
            chkCount(ctx, TEST_MAX);
            
            /* Check suspend... */
            
            TPTRANID tx1 =  ctx.tpsuspend(0);
            assertNotNull(tx1);
            
            ctx.tpbegin(20, 0);
            
            chkCount(ctx, 0);
            for (int j=0; j<10; j++)
            {
                /* call the server, insert some data */
                ub.Bchg(test.T_LONG_FLD, 0, (long)j);
                ub.Bchg(test.T_STRING_FLD, 0, String.format("Name %d", j));
                ub.Bchg(test.T_STRING_2_FLD, 0, String.format("City %d", j));

                /* well we shall suspend our side here... */
                ub = (TypedUbf)ctx.tpcall("DoTran", ub, AtmiConst.TPTRANSUSPEND);
            }
            
            chkCount(ctx, 10);
            ctx.tpabort(0);
            
            
            /* resume back original transaction... */
            ctx.tpresume(tx1, 0);
            chkCount(ctx, TEST_MAX);
            
            /* lets abort */
            ctx.tpcommit(0);
            chkCount(ctx, TEST_MAX);
            
            if (shutdownReq) {
                ctx.tplogInfo("Shutting down by request....");
                break;
            }
        }
        
        try {
            conn.close();
        }  catch (SQLException e) {
            ctx.tplogex(AtmiConst.LOG_ERROR, "Failed to close conn", e);
            assertEquals(null, e);
        }
        
        ctx.tplogError("Ending UP!");
        /*
        ctx.tpclose();
        ctx.tpterm();
         */
        
        ub.cleanup();
        ctx.cleanup();
        //System.console().printf("Done with shut...\n");
        //shutdownDone = true;
        
        //throw new RuntimeException("TEST");
    }
    
}
