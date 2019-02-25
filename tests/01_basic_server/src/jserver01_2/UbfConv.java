import org.endurox.*;

/**
 * UBF Conversational service
 */
public class UbfConv implements Service {

    /**
     * COnversational testing server. The scenario will be following:
     * - endpoint connects
     * - we send 100 msg (UBF messages)
     * - we receive 100 msg
     * - perform tpreturn
     * @param ctx Atmi context
     * @param svcinfo service info
     */
    public void tpService(AtmiCtx ctx, TpSvcInfo svcinfo) {
        
        ctx.tplogDebug("tpService UbfConv called");
        /* Allocate the correct buffer... */
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 0);
        int cd = svcinfo.getCd();
        long revent;
        
        for (int i=0; i<100; i++)
        {
            ub.Bchg(test.T_LONG_FLD, 0, i+100);
            
            if (i==99)
            {
                revent = ctx.tpsend(cd, ub, AtmiConst.TPRECVONLY);
            }
            else
            {
                revent = ctx.tpsend(cd, ub, 0L);
            }
            
            if (0!=revent)
            {
                ctx.tplogError("Got %d event while receiving", revent);
                /* just tpreturn and that it */
                ctx.tpreturn(AtmiConst.TPFAIL, 0, ub, 0);
                return; /* <<<< RETURN ! */
            }
        }
        
        /* Now recieve 100 msgs  */
        for (int i=0; i<100; i++)
        {
            TprecvResult rec;
            rec = ctx.tprecv(cd, ub, 0);
            
            if (0!=rec.getRevent())
            {
                /* terminate the conversation */
                ctx.tplogError("Got %d event while sending", rec.getRevent());
                
                /* just tpreturn and that it */
                ctx.tpreturn(AtmiConst.TPFAIL, 0, ub, 0);
                
                return; /* <<<< RETURN ! */
            }
                        
            ub = (TypedUbf)rec.getBuffer();
            
            long val = ub.BgetLong(test.T_LONG_2_FLD, 0);
            
            if (val!=i+200)
            {
                ctx.tplogError("TESTERROR, expected %d at receive, but got %d", 
                        i+200, val);
                ctx.tpreturn(AtmiConst.TPFAIL, 0, null, 0);
                return; /* <<<< RETURN ! */
            }
        }
        ctx.tpreturn(AtmiConst.TPSUCCESS, 0, null, 0);
    }
}
