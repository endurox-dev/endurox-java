import org.endurox.*;

/**
 * Null response service
 */
public class NullRspFail implements Service {

    /**
     * Responds with null buffer
     * add some text to buffer
     * @param ctx Atmi context
     * @param svcinfo service info
     */
    public void tpService(AtmiCtx ctx, TpSvcInfo svcinfo) {
        
        ctx.tplogDebug("tpService NullRspFail called");
        
        ctx.tpreturn(AtmiConst.TPFAIL, 63, null, 0);
    }
}
