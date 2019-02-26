import org.endurox.*;

/**
 * Universal conversation tester
 */
public class Convsv implements Service {

        /**
         * Main logic implementation
         * @param ctx
         * @param svcinfo 
         */
        public void main(AtmiCtx ctx, TpSvcInfo svcinfo) {
        
        String svcnm = svcinfo.getName();
        String fnnm = svcinfo.getFname();
        String btype = svcnm.substring(0, svcnm.length() - 4);
        String subtype = "";        
        int cd = svcinfo.getCd();
        long revent;
        
        //String btype = svcinfo.getName().substr
        //Strip off "CONV"
        
        if (btype.equals("VIEW")) {
            subtype = "JVIEW2";
        }
        
        ctx.tplogDebug("tpService [%s]/[%s] called btype [%s]/[%s]", 
                svcnm, fnnm, btype, subtype);
        
        /* Allocate the correct buffer... */
        TypedBuffer b = ctx.tpalloc(btype, subtype, 1024);
        
        /* we send a stuff for 100x times */
        for (int i=0; i<100; i++)
        {
            int snd = i + 3;
            
            if (btype.equals("STRING")) {
                TypedString s = (TypedString)b;
                s.setString(Integer.toString(snd));
            }
            else if (btype.equals("JSON")) {
                /* it is the same string... */
                TypedJson j = (TypedJson)b;
                
                /* I know it is not a json, but basically it is string transport... */
                j.setJSON(Integer.toString(snd));
            }
            else if (btype.equals("CARRAY")) {
                TypedCarray c = (TypedCarray)b;
                
                byte bb = (byte)snd;
                c.setBytes(new byte[] {bb});
            }
            else if (btype.equals("UBF")) {
                TypedUbf u = (TypedUbf)b;
                u.Bchg(test.T_LONG_FLD, 0, snd);
            }
            else if (btype.equals("VIEW")) {
                /* nothing to send.. */
            }
            else if (btype.equals("NULL")) {
                /* nothing to send.. */
            }
            
            if (i==99)
            {
                revent = ctx.tpsend(cd, b, AtmiConst.TPRECVONLY);
            }
            else
            {
                revent = ctx.tpsend(cd, b, 0L);
            }
            
            if (0!=revent)
            {
                ctx.tplogError("TESTERROR ! Got %d event while receiving", revent);
                /* just tpreturn and that it */
                ctx.tpreturn(AtmiConst.TPFAIL, 0, null, 0);
                return; /* <<<< RETURN ! */
            }
        }
        
        /* Now recieve 100 msgs  */
        for (int i=0; i<100; i++)
        {
            TprecvResult rec = null;
            long gotval;
            
            //No exception is expected here...
            rec = ctx.tprecv(cd, b, 0);
            
            b = rec.getBuffer();
            
            if (null==b) {
                btype = "NULL";
            } else {
                btype = b.tptypes().getType();
            }
        
            /* well here we can receive any type...
             * thus service type does not matter, but buffer type recieved
             */
            if (btype.equals("STRING")) {
                TypedString s = (TypedString)b;
                gotval = Integer.parseInt(s.getString());
            }
            else if (btype.equals("JSON")) {
                /* it is the same string... */
                TypedJson j = (TypedJson)b;
                gotval = Integer.parseInt(j.getJSON());
            }
            else if (btype.equals("CARRAY")) {
                TypedCarray c = (TypedCarray)b;
                gotval = c.getBytes()[0];
            }
            else if (btype.equals("UBF")) {
                TypedUbf u = (TypedUbf)b;
                gotval = u.BgetLong(test.T_LONG_FLD, 0);
            }
            else if (btype.equals("VIEW")) {
                gotval = i + 7;
            }
            else if (btype.equals("NULL")) {
                gotval = i + 7;
            }
            else {
                gotval = AtmiConst.FAIL;
            }
            
            if (i + 7 != gotval) {
                ctx.tplogError("TESTERROR ! Extected %d got %d", 
                        i + 7, gotval);
                ctx.tpreturn(AtmiConst.TPFAIL, 0, null, 0);
                return; /* <<<< Return ! */
            }
        }
        
        ctx.tpreturn(AtmiConst.TPSUCCESS, 0, null, 0);
    }
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
        
        try {
            this.main(ctx, svcinfo);
        } 
        catch (ClassCastException e) {
            ctx.tplogex(AtmiConst.LOG_INFO, "Type cast exception - ok "
                    + "for alien types", e);
            ctx.tpreturn(AtmiConst.TPFAIL, 0, null, 0);
        }
    }
}
