import org.endurox.*;
import java.util.*;

public class JServer01 implements Server, Service {

    public void tpService(AtmiCtx ctx, TpSvcInfo svcinfo) {
        ctx.tplogDebug("tpService/HELLO called");
        ctx.tpreturn(AtmiConstants.TPSUCCESS, 0, svcinfo.getData(), 0);
    }

    public int tpSvrInit(AtmiCtx ctx, String [] argv) {
        ctx.tplogDebug("Into tpSvrInit()");
        ctx.tpadvertise("HELLOSVC", "HELLOFN", this);
        return AtmiConstants.SUCCEED;
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
