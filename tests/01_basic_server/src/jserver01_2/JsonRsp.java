import org.endurox.*;

/**
 * JSON Response service
 */
public class JsonRsp implements Service {

    /**
     * Responds with JSON buffer
     * add some text to buffer
     * @param ctx Atmi context
     * @param svcinfo service info
     */
    public void tpService(AtmiCtx ctx, TpSvcInfo svcinfo) {
        
        ctx.tplogDebug("tpService StringRsp called");

        TypedJson ret = (TypedJson)ctx.tpalloc("JSON", null, 100);
        ret.setJSON("{}");
        
        ctx.tpreturn(AtmiConst.TPSUCCESS, 0, ret, 0);
    }
}
