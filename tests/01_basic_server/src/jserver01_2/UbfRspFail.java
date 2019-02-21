import org.endurox.*;

/**
 * UBF Response service
 */
public class UbfRspFail implements Service {

    /**
     * Responds with null buffer
     * add some text to buffer
     * @param ctx Atmi context
     * @param svcinfo service info
     */
    public void tpService(AtmiCtx ctx, TpSvcInfo svcinfo) {
        
        ctx.tplogDebug("tpService UbfRspFail called");
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 0);
        ub.Bchg(test.T_STRING_10_FLD, 5, "HELLO UBF FROM SERVICE");
        ctx.tpreturn(AtmiConst.TPFAIL, 63, ub, 0);
    }
}
