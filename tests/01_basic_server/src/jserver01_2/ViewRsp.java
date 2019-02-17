import org.endurox.*;

/**
 * View response service
 */
public class ViewRsp implements Service {

    /**
     * Responds with VIEW buffer
     * add some text to buffer
     * @param ctx Atmi context
     * @param svcinfo service info
     */
    public void tpService(AtmiCtx ctx, TpSvcInfo svcinfo) {
        
        ctx.tplogDebug("tpService StringRsp called");

        TypedView ret = (TypedView)ctx.tpalloc("VIEW", "JVIEW2", 0);
        
        /* use defaults and respond */
        ctx.tpreturn(AtmiConst.TPSUCCESS, 0, ret, 0);
    }
}
