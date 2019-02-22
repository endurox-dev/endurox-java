import org.endurox.*;

public class UbfOk2 implements Service {

    public void tpService(AtmiCtx ctx, TpSvcInfo svcinfo) {
        
        ctx.tplogDebug("UBF OK Service");

        TypedUbf ub = (TypedUbf)svcinfo.getData();
        
        /* set new size */
        ub.tprealloc(1024);
        
        /* Add new value */
        ub.Badd(test.T_STRING_FLD, "UBF OK 2");
           
        ctx.tpreturn(AtmiConst.TPSUCCESS, 0, svcinfo.getData(), 0);
    }

}
