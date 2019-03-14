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
        //This should provide server error back not?
        ctx.tpadvertise("EXCEPT", "ExceptRsp", new ExceptRsp());
        
        
        //This should provide server error back not?
        //Succeed responders:
        ctx.tpadvertise("NULLRSP", "NullRspSvc", new NullRsp());
        ctx.tpadvertise("JSONRSP", "JsonRsp", new JsonRsp());
        ctx.tpadvertise("STRINGRSP", "StringRsp", new StringRsp());
        ctx.tpadvertise("UBFRSP", "UbfRsp", new UbfRsp());
        ctx.tpadvertise("VIEWRSP", "ViewRsp", new ViewRsp());
        ctx.tpadvertise("CARRAYRSP", "CarrayRsp", new CarrayRsp());
        
        //Fail responders + tpurcode:
        ctx.tpadvertise("NULLRSPFAIL", "NullRspFail", new NullRspFail());
        ctx.tpadvertise("JSONRSPFAIL", "JsonRspFail", new JsonRspFail());
        ctx.tpadvertise("STRINGRSPFAIL", "StringRspFail", new StringRspFail());
        ctx.tpadvertise("UBFRSPFAIL", "UbfRspFail", new UbfRspFail());
        ctx.tpadvertise("VIEWRSPFAIL", "ViewRspFail", new ViewRspFail());
        ctx.tpadvertise("CARRAYRSPFAIL", "CarrayRspFail", new CarrayRspFail());
        
        //Service errors, no response
        ctx.tpadvertise("NULLRSPFAIL10", "NullRspFail10", new NullRspFail10());
        ctx.tpadvertise("JSONRSPFAIL10", "JsonRspFail10", new JsonRspFail10());
        ctx.tpadvertise("STRINGRSPFAIL10", "StringRspFail10", new StringRspFail10());
        ctx.tpadvertise("UBFRSPFAIL10", "UbfRspFail10", new UbfRspFail10());
        ctx.tpadvertise("VIEWRSPFAIL10", "ViewRspFail10", new ViewRspFail10());
        ctx.tpadvertise("CARRAYRSPFAIL10", "CarrayRspFail10", new CarrayRspFail10());
        
        //Forwarder
        ctx.tpadvertise("UBFOKFWD1", "UbfOkFwd1", new UbfOkFwd1());
        
        //Conversations with buffers
        ctx.tpadvertise("NULLCONV", "Convsv", new Convsv());
        ctx.tpadvertise("JSONCONV", "Convsv", new Convsv());
        ctx.tpadvertise("STRINGCONV", "Convsv", new Convsv());
        ctx.tpadvertise("UBFCONV", "Convsv", new Convsv());
        ctx.tpadvertise("VIEWCONV", "Convsv", new Convsv());
        ctx.tpadvertise("CARRAYCONV", "Convsv", new Convsv());

        ctx.tpadvertise("DROPRSP", "DropRsp", new DropRsp());
        
        //Notification servers:
        ctx.tpadvertise("GLOBNOTIF", "GlobNotif", new GlobNotif());
        ctx.tpadvertise("GLOBBROAD", "GlobBroad", new GlobBroad());
        
        
        
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
