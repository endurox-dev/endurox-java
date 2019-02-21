import org.endurox.*;

/**
 * UBF Error time-out Response service
 */
public class UbfToutRsp implements Service {

    /**
     * Responds with null buffer
     * add some text to buffer
     * @param ctx Atmi context
     * @param svcinfo service info
     */
    public void tpService(AtmiCtx ctx, TpSvcInfo svcinfo) {
        
        ctx.tplogDebug("tpService UbfRsp called");
        
        //Just ignore response...
    }
}
