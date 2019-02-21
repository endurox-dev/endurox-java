import org.endurox.*;

/**
 * String response service
 */
public class StringRspFail implements Service {

    /**
     * Responds with null buffer
     * add some text to buffer
     * @param ctx Atmi context
     * @param svcinfo service info
     */
    public void tpService(AtmiCtx ctx, TpSvcInfo svcinfo) {
        
        ctx.tplogDebug("tpService StringRspFail called");

        TypedString ret = (TypedString)ctx.tpalloc("STRING", null, 100);
        
        /* We need to set string & get string..! */
        
        ret.setString("HELLO FROM SERVICE");
        
        ctx.tpreturn(AtmiConst.TPFAIL, 63, ret, 0);
    }
}
