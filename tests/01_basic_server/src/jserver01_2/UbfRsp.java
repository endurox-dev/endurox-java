import org.endurox.*;

/**
 * UBF Response service
 */
public class UbfRsp implements Service {

    /**
     * Responds with null buffer
     * add some text to buffer
     * @param ctx Atmi context
     * @param svcinfo service info
     */
    public void tpService(AtmiCtx ctx, TpSvcInfo svcinfo) {
        
        ctx.tplogDebug("tpService UbfRsp called");
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 0);
        ub.Bchg(test.T_STRING_10_FLD, 5, "HELLO UBF FROM SERVICE");
        ctx.tpreturn(AtmiConst.TPSUCCESS, 0, ub, 0);
    }
}
