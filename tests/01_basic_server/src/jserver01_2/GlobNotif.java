import org.endurox.*;

/**
 * Forward the incoming buffer to client notification...
 */
public class GlobNotif implements Service {

    /**
     * Send the buffer to client notification
     */
    public void tpService(AtmiCtx ctx, TpSvcInfo svcinfo) {
        
        ctx.tplogDebug("tpService GlobNotif called");
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 0);
        
        ub.Bchg(test.T_STRING_10_FLD, 5, "HELLO UBF FROM SERVICE");
        
        ctx.tpnotify(svcinfo.getCltId(), ub, 0);
        
        ctx.tpreturn(AtmiConst.TPSUCCESS, 0, ub, 0);
    }
}
