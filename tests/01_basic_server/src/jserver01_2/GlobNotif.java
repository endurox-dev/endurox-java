import org.endurox.*;

/**
 * Forward the incoming buffer to client notification...
 */
public class GlobNotif implements Service {

    /**
     * Send the buffer to client notification
     */
    public void tpService(AtmiCtx ctx, TpSvcInfo svcinfo) {
        
        int ret = AtmiConst.TPSUCCESS;
        TypedBuffer buf = svcinfo.getData();
        
        ctx.tplogDebug("tpService GlobNotif called");
        
        TpTypesResult typ = null;
        
        
        if (buf!=null) {
            typ = buf.tptypes();
        }
        
        if (null==buf) {
            ctx.tpnotify(svcinfo.getCltId(), null, 0);
        } 
        else if (typ.getType().equals("STRING")) {
            TypedString s = (TypedString)ctx.tpalloc("STRING", "", 0);
            s.setString("HELLO NOTIF");
            ctx.tpnotify(svcinfo.getCltId(), s, 0);
        }  
        else if (typ.getType().equals("JSON")) {
            TypedJson s = (TypedJson)ctx.tpalloc("JSON", "", 0);
            s.setJSON("{}");
            ctx.tpnotify(svcinfo.getCltId(), s, 0);
        } 
        else if (typ.getType().equals("CARRAY")) {
            TypedCarray c = (TypedCarray)ctx.tpalloc("CARRAY", "", 100);
            c.setBytes(new byte [] {0, 1, 2, 3, 4, 5, 6});
            ctx.tpnotify(svcinfo.getCltId(), c, 0);
        }
        else if (typ.getType().equals("UBF")) {
            TypedUbf b = (TypedUbf)ctx.tpalloc("UBF", "", 100);
            b.Bchg(test.T_STRING_10_FLD, 5, "HELLO UBF FROM SERVICE");
            ctx.tpnotify(svcinfo.getCltId(), b, 0);
        }
        else if (typ.getType().equals("VIEW")) {
            TypedView v = (TypedView)ctx.tpalloc("VIEW", "JVIEW2", 100);
            ctx.tpnotify(svcinfo.getCltId(), v, 0);
        }
        else {
            ctx.tplogError("Invalid Type: [%s]", typ.getSubType());
            ret = AtmiConst.TPFAIL;
        }
        
        ctx.tpreturn(ret, 0, null, 0);
    }
}
