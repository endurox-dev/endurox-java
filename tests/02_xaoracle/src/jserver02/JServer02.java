import org.endurox.*;
import java.util.*;

public class JServer02 implements Server, Service {


    /**
     * Do some transaction work
     */
    public void tpService(AtmiCtx ctx, TpSvcInfo svcinfo) {
        
        ctx.tplogDebug("tpService/DoTran called");

        TypedUbf ub = (TypedUbf)svcinfo.getData();

        /* TODO: Add something to database */

        
        ctx.tpreturn(AtmiConst.TPSUCCESS, 0, svcinfo.getData(), 0);
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

