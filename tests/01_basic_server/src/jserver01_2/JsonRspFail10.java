import org.endurox.*;

/**
 * JSON Response service
 */
public class JsonRspFail10 implements Service {

    /**
     * Responds with JSON buffer
     * add some text to buffer
     * @param ctx Atmi context
     * @param svcinfo service info
     */
    public void tpService(AtmiCtx ctx, TpSvcInfo svcinfo) {
        
        ctx.tplogDebug("tpService JsonRspFail10 called");

        TypedJson ret = (TypedJson)ctx.tpalloc("JSON", null, 100);
        ret.setJSON("{}");
        
        ctx.tpreturn(AtmiConst.TPFAIL, AtmiConst.TPESVCERR, ret, AtmiConst.TPSOFTERR);
    }
}
