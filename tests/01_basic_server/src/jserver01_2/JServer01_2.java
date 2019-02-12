import org.endurox.*;
import java.util.*;

public class JServer01_2 implements Server, Service {

    public void tpService(AtmiCtx ctx, TpSvcInfo svcinfo) {
        
        ctx.tplogDebug("tpService/ECHOSVC called");

        TypedUbf ub = (TypedUbf)svcinfo.getData();
        
        /* echo the field value... */
        ub.Bchg(test.T_STRING_2_FLD, 0, ub.BgetString(test.T_STRING_FLD, 0));
     
        
        /* TODO: Test here expression handling deallocations...!
         * might get a leak...!
         */
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
        
        ctx.tpadvertise("ECHOSVC", "ECHOSVC", this);
        //Allocate NULL service
        ctx.tpadvertise("NULL", "NullSvc", new NullSvc());
        
        ctx.tpadvertise("NULLRSP", "NullRspSvc", new NullRspSvc());
        
        //TODO: Process exception here. If failed, then report fail to ndrx
        //for failed startup...
        return AtmiConst.SUCCEED;
    }
    
    /**
     * Shutdown of XATMI server
     * @param ctx 
     */
    public void tpSvrDone(AtmiCtx ctx) {
        ctx.tplogDebug("Into tpSvrDone()");
    }
    
    /**
     * Main entry of XATMI server
     * @param args 
     */
    public static void main(String[] args) {
    
        AtmiCtx ctx = new AtmiCtx();

        System.out.println("About to create ...");
        JServer01_2 server = new JServer01_2();

        System.out.println("About to log ...");
        ctx.tplogInfo("Started server with: %s", Arrays.toString(args));
        
        /* create new local class */
        System.out.println("About to tpRun ...");
        
        /*
         * Run main Enduro/X XATMI Server's main loop
         */
        ctx.tprun(server);
    }
}
