import org.endurox.*;

/**
 * Null response service
 */
public class NullRsp implements Service {

    /**
     * Responds with null buffer
     * add some text to buffer
     * @param ctx Atmi context
     * @param svcinfo service info
     */
    public void tpService(AtmiCtx ctx, TpSvcInfo svcinfo) {
        
        ctx.tplogDebug("tpService NullRspSvc called");
        
        

        ctx.tpreturn(AtmiConst.TPSUCCESS, 0, null, 0);
    }
}
