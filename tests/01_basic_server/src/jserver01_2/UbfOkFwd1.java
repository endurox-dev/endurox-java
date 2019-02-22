import org.endurox.*;

/**
 * UBF Forward service
 */
public class UbfOkFwd1 implements Service {

    /**
     * Forward request to UBFOK2 service
     * add some text to buffer
     * @param ctx Atmi context
     * @param svcinfo service info
     */
    public void tpService(AtmiCtx ctx, TpSvcInfo svcinfo) {
        
        ctx.tplogDebug("tpService UBF Forwarder 1");
        TypedUbf ub = (TypedUbf)svcinfo.getData();
        
        /* have some space */
        ub.tprealloc(1024);
        
        ub.Badd(test.T_STRING_FLD, "UBF FWD 1");

        ctx.tpforward("UBFOK2", ub, 0);
        
    }
}
