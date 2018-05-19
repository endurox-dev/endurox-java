import org.endurox.*;

public class JServer01 implements Server {

    public int tpSvrInit(AtmiCtx ctx, String [] argv) {
        ctx.tpLogDebug("Into tpSvrInit()");
        return AtmiConstants.SUCCEED;
    }
    
    public void tpSvrDone(AtmiCtx ctx) {
        ctx.tpLogDebug("Into tpSvrDone()");
    }
    
    public static void main(String[] args) {
    
        AtmiCtx ctx = new AtmiCtx();
        JServer01 server = new JServer01();
        
        /* create new local class */
        ctx.TpRun(server, args);
    }
}
