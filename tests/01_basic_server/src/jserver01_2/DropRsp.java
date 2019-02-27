import org.endurox.*;

/**
 * Drop message
 */
public class DropRsp implements Service {

    /**
     * Responds with VIEW buffer
     * add some text to buffer
     * @param ctx Atmi context
     * @param svcinfo service info
     */
    public void tpService(AtmiCtx ctx, TpSvcInfo svcinfo) {
        
        ctx.tplogDebug("tpService DropRsp called");

    }
}
