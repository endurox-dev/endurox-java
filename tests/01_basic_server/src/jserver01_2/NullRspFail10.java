import org.endurox.*;

/**
 * Null response service
 */
public class NullRspFail10 implements Service {

    /**
     * Responds with null buffer
     * add some text to buffer
     * @param ctx Atmi context
     * @param svcinfo service info
     */
    public void tpService(AtmiCtx ctx, TpSvcInfo svcinfo) {
        
        ctx.tplogDebug("tpService NullRspFail10 called");
        
        ctx.tpreturn(AtmiConst.TPFAIL, AtmiConst.TPESVCERR, null, AtmiConst.TPSOFTERR);
    }
}
