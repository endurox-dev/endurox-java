import org.endurox.*;

/**
 * Forward the incoming buffer to client notification...
 */
public class GlobBroad implements Service {

    
    /**
     * Perform brodcast
     * @param lmid
     * @param usrname
     * @param cltname 
     */
    void doBroadcast(AtmiCtx ctx, TypedBuffer buf) {
        
        /* +1 */
        ctx.tpbroadcast(null, null, null, buf, 0);
        /* +1 */
        ctx.tpbroadcast(null, null, "jexunit01b", buf, 0);
        /* +1 */
        ctx.tpbroadcast(null, null, "unit", buf, AtmiConst.TPREGEXMATCH);
        /* +0 */
        ctx.tpbroadcast(null, null, "hello", buf, AtmiConst.TPREGEXMATCH);
        /* +1 */
        ctx.tpbroadcast("[0-9]*", null, "unit", buf, AtmiConst.TPREGEXMATCH);
            
    }
    /**
     * Send the buffer to client notification
     */
    public void tpService(AtmiCtx ctx, TpSvcInfo svcinfo) {
        
        int ret = AtmiConst.TPSUCCESS;
        TypedBuffer buf = svcinfo.getData();
        
        ctx.tplogDebug("tpService GlobBroad called");
        
        TpTypesResult typ = null;
        
        
        if (buf!=null) {
            typ = buf.tptypes();
        }
        
        if (null==buf) {
            doBroadcast(ctx, null);
        } 
        else if (typ.getType().equals("STRING")) {
            TypedString s = (TypedString)ctx.tpalloc("STRING", "", 0);
            s.setString("HELLO NOTIF");
            doBroadcast(ctx, s);
        }  
        else if (typ.getType().equals("JSON")) {
            TypedJson s = (TypedJson)ctx.tpalloc("JSON", "", 0);
            s.setJSON("{}");
            doBroadcast(ctx, s);
        } 
        else if (typ.getType().equals("CARRAY")) {
            TypedCarray c = (TypedCarray)ctx.tpalloc("CARRAY", "", 100);
            c.setBytes(new byte [] {0, 1, 2, 3, 4, 5, 6});
            doBroadcast(ctx, c);
        }
        else if (typ.getType().equals("UBF")) {
            TypedUbf b = (TypedUbf)ctx.tpalloc("UBF", "", 100);
            b.Bchg(test.T_STRING_10_FLD, 5, "HELLO UBF FROM SERVICE");
            doBroadcast(ctx, b);
        }
        else if (typ.getType().equals("VIEW")) {
            TypedView v = (TypedView)ctx.tpalloc("VIEW", "JVIEW2", 100);
            doBroadcast(ctx, v);
        }
        else {
            ctx.tplogError("Invalid Type: [%s]", typ.getSubType());
            ret = AtmiConst.TPFAIL;
        }
        
        ctx.tpreturn(ret, 0, null, 0);
    }
}
