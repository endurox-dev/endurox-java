import java.sql.Connection;
import java.sql.SQLException;
import java.sql.Statement;
import org.endurox.*;

public class JServer02 implements Server, Service {


    Connection conn = null;
    Statement statement = null;
    
    /**
     * Do some transaction work
     */
    public void tpService(AtmiCtx ctx, TpSvcInfo svcinfo) {
        
        int ret = AtmiConst.TPSUCCESS;
        
        
        ctx.tplogDebug("tpService/DoTran called");

        TypedUbf ub = (TypedUbf)svcinfo.getData();

        /* TODO: Add something to database */
        
        long id = ub.BgetShort(test.T_LONG_FLD, 0);
        String name = ub.BgetString(test.T_STRING_FLD, 0);
        String city = ub.BgetString(test.T_STRING_2_FLD, 0);
        
        String sql = String.format("insert into EXJTEST(customer_id, customer_name, city) values (%d, '%s', '%s')", 
                id, name, city);
        ctx.tplogInfo("Running stmt: [%s]", sql);
        
        try {
            // insert the data
            statement.executeUpdate(sql);
        }
        catch (Exception e) {
            ctx.tplogex(AtmiConst.LOG_ERROR, "Failed to run update", e);
            ret = AtmiConst.TPFAIL;
        }
        
        ctx.tpreturn(ret, 0, svcinfo.getData(), 0);
    }

    /**
     * Initialize the service (callback by Enduro/X)
     * @param ctx Atmi Context
     * @param argv command line arguments (including binary name)
     * @return SUCCEED/FAIL
     */
    public int tpSvrInit(AtmiCtx ctx, String [] argv) {

        /* argv[0] = "HELLO"; */
        ctx.tplogDebug("Into tpSvrInit");

        /* Open Connection */
        ctx.tpopen();
        
        try {
            conn = ctx.getConnection();
            // create a Statement from the connection
            statement = conn.createStatement();
            
        } catch (Exception e) {
            ctx.tplogex(AtmiConst.LOG_ERROR, "Failed to get SQL Connect or STMT", e);
            throw new RuntimeException(e);
        }
        
        //Notification servers:
        ctx.tpadvertise("DoTran", "DoTran", this);
        
        //for failed startup...
        return AtmiConst.SUCCEED;
    }
    
    /**
     * Shutdown of XATMI server
     * @param ctx 
     */
    public void tpSvrDone(AtmiCtx ctx) {
        ctx.tplogDebug("Into tpSvrDone()");
        
        try {
            statement.close();
        } catch (Exception e) {
            ctx.tplogex(AtmiConst.LOG_ERROR, "Failed to close statement", e);
        }
        
        try {
            conn.close();
        } catch (Exception e) {
            ctx.tplogex(AtmiConst.LOG_ERROR, "Failed to close conn", e);
        }
        
        ctx.tpclose();
    }
    
    /**
     * Main entry of XATMI server
     * @param args 
     */
    public static void main(String[] args) {
    
        AtmiCtx ctx = new AtmiCtx();

        System.out.println("About to JServer02 ...");
        JServer02 server = new JServer02();
        
        /*
         * Run main Enduro/X XATMI Server's main loop
         */
        ctx.tprun(server);
    }
}

