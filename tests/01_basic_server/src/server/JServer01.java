import org.endurox.*;
import java.util.*;

public class JServer01 implements Server, Service {

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

    public int tpSvrInit(AtmiCtx ctx, String [] argv) {
        ctx.tplogDebug("Into tpSvrInit()");
        ctx.tpadvertise("ECHOSVC", "ECHOSVC", this);
        return AtmiConst.SUCCEED;
    }
    
    public void tpSvrDone(AtmiCtx ctx) {
        ctx.tplogDebug("Into tpSvrDone()");
    }
    
    public static void main(String[] args) {
    
        AtmiCtx ctx = new AtmiCtx();

        System.out.println("About to create ...");
        JServer01 server = new JServer01();

        System.out.println("About to log ...");
        ctx.tplogInfo("Started server with: %s", Arrays.toString(args));
        
        /* create new local class */
        System.out.println("About to tpRun ...");
        ctx.tprun(server);
    }
}
