import org.endurox.*;

/**
 * Null response service
 */
public class CarrayRspFail implements Service {

    /**
     * Respond with carray buffer
     * add some text to buffer
     * @param ctx Atmi context
     * @param svcinfo service info
     */
    public void tpService(AtmiCtx ctx, TpSvcInfo svcinfo) {
        
        ctx.tplogDebug("tpService CarrayRspFail called");

        TypedCarray ret = ctx.newCarray(new byte[]{1, 2, 5, 2, 1, 2, 3, 126});
        
        ctx.tpreturn(AtmiConst.TPFAIL, 63, ret, 0);
    }
}
