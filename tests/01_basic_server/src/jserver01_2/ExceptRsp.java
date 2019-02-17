import org.endurox.*;

/**
 * Exception responder
 */
public class ExceptRsp implements Service {

    /**
     * Respond with exception to caller
     * @param ctx Atmi context
     * @param svcinfo service info
     */
    public void tpService(AtmiCtx ctx, TpSvcInfo svcinfo) {
        ctx.tplogDebug("ExceptRsp exception responder - we should reply "
                + "with service failure not?");
        throw new RuntimeException("Service generates exception!");
    }
}
