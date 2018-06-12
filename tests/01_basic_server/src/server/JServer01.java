import org.endurox.*;
import java.util.*;

public class JServer01 implements Server, Service {

    public void tpService(AtmiCtx ctx, TpSvcInfo svcinfo) {
        ctx.tpLogDebug("tpService/HELLO called");
        ctx.tpReturn(AtmiConstants.TPSUCCESS, 0, svcinfo.getData(), 0);
    }

    public int tpSvrInit(AtmiCtx ctx, String [] argv) {
        ctx.tpLogDebug("Into tpSvrInit()");
        ctx.tpAdvertise("HELLOSVC", "HELLOFN", this);
        return AtmiConstants.SUCCEED;
    }
    
    public void tpSvrDone(AtmiCtx ctx) {
        ctx.tpLogDebug("Into tpSvrDone()");
    }
    
    public static void main(String[] args) {
    
        AtmiCtx ctx = new AtmiCtx();
        JServer01 server = new JServer01();

        ctx.tpLogInfo("Started server with: %s", Arrays.toString(args));
        
        /* create new local class */
        ctx.tpRun(server, args);
    }
}
