import org.endurox.*;

/**
 * Null response service
 */
public class StringRsp implements Service {

    /**
     * Responds with null buffer
     * add some text to buffer
     * @param ctx Atmi context
     * @param svcinfo service info
     */
    public void tpService(AtmiCtx ctx, TpSvcInfo svcinfo) {
        
        ctx.tplogDebug("tpService StringRsp called");

        TypedString ret = (TypedString)ctx.tpalloc("STRING", null, 100);
        
        /* TODO: We need to set string & get string..! */
        
        ctx.tpreturn(AtmiConst.TPSUCCESS, 0, ret, 0);
    }
}
