import org.endurox.*;

public class NullSvc implements Service {

    /**
     * Null service entry
     * Checks for null data, if not fail
     * add some text to buffer
     * @param ctx Atmi context
     * @param svcinfo service info
     */
    public void tpService(AtmiCtx ctx, TpSvcInfo svcinfo) {
        
        ctx.tplogDebug("tpService NullSvc called");

        if (null==svcinfo.getData()) {
            TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", null, 0);

            /* echo the field value... */
            ub.Bchg(test.T_STRING_2_FLD, 0, "HELLO NULL");
            
            ctx.tpreturn(AtmiConst.TPSUCCESS, 0, ub, 0);
            
        } else {
            ctx.tplogError("Buffer not NULL!");
            ctx.tpreturn(AtmiConst.TPFAIL, 0, svcinfo.getData(), 0);
        }
    }
}
