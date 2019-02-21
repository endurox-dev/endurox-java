import org.endurox.*;

/**
 * View response service
 */
public class ViewRspFail10 implements Service {

    /**
     * Responds with VIEW buffer
     * add some text to buffer
     * @param ctx Atmi context
     * @param svcinfo service info
     */
    public void tpService(AtmiCtx ctx, TpSvcInfo svcinfo) {
        
        ctx.tplogDebug("tpService ViewRspFail called");

        TypedView ret = (TypedView)ctx.tpalloc("VIEW", "JVIEW2", 0);
        
        /* use defaults and respond */
        ctx.tpreturn(AtmiConst.TPFAIL, AtmiConst.TPESVCERR, ret, AtmiConst.TPSOFTERR);
    }
}
