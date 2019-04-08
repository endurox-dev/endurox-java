import static org.junit.Assert.*;
import org.endurox.*;
import org.endurox.exceptions.AtmiException;
/**
 * Async tpcall tests
 */
public class Commons {
             
    /**
     * Cross validate 
     * @param svc Service name
     * @param input_type intput buffer type
     * @param input_sub input sub-buffer type
     * @param output_type output buffer type
     * @param output_sub output buffer sub-type
     * @param flags call flags
     * @param tpcallerr return error code/from exception
     * @param validate shall we validate the error
     * @param asyncMode if false use tpcall, if true use tpacall/tpgetrply
     * @return SUCCEED/FAIL
     */
    void testerX(AtmiCtx ctx, String svc, String input_type, String input_sub, 
        String output_type, String output_sub, long flags, int tpcallerr, 
        boolean validate, boolean asyncMode) {
        
        TypedBuffer b = null;
        TypedBuffer b_ret;
        int excpt_err = 0;
        int cd = 0;
        
        ctx.tplogInfo("svc: [%s] input_type: [%s] input_sub: [%s] output_type: "+
            "[%s] output_sub: [%s] flags: %d tpcallerr: %d validate: %b async: %b",
            svc, input_type, input_sub, output_type, output_sub, flags, 
            tpcallerr, validate,asyncMode);
        
        if (input_type.equals("NULL")) {
            b = null;
        } 
        else if (input_type.equals("STRING")) {
            b = ctx.newString("This is unit test");
        }
        else if (input_type.equals("JSON")) {
            b = ctx.newJson("[]");
        }
        else if (input_type.equals("CARRAY")) {
            b = ctx.newCarray(new byte [] {9, 4, 3, 2});
        }
        else if (input_type.equals("VIEW")) {
            b = ctx.tpalloc(input_type, input_sub, 0);
        }
        else if (input_type.equals("UBF")) {
            TypedUbf u;
            b = ctx.tpalloc(input_type, "", 0);
            
            u = (TypedUbf)b;   
            u.Bchg(test.T_STRING_5_FLD, 11, "HELLO 11");
        }
        
        /* call the service */
        
        if (asyncMode) {
            cd = ctx.tpacall(svc, b, flags);
        }
        
        try {
            
            if (asyncMode) {
                
                TpgetrplyResult ares = ctx.tpgetrply(cd, b, flags);
                
                /* get reply buffer */
                b_ret = ares.getBuffer();
                
            } else {
                b_ret = ctx.tpcall(svc, b, flags);
            }
            
        } catch (AtmiException e) {
            
            ctx.tplogex(AtmiConst.LOG_ERROR, "Got exception", e);
            excpt_err = e.getErrorCode();
            /*
            result will already contain the filtered either original
            or received buffer
            if (null!=e.getReplyBuffer())
            {
                b_ret = e.getReplyBuffer();
            } else {
                b_ret = b;
            }
            */
            b_ret = e.getReplyBuffer();
            
        }
        
        /* validate the response */
        assertEquals(svc, tpcallerr, excpt_err);
        
        if (tpcallerr==AtmiConst.TPESVCFAIL || 
                /* copy from previous call.. */
                tpcallerr==AtmiConst.TPESVCERR)
        {
            assertEquals(63, ctx.tpurcode());
        } else {
            assertEquals(0, ctx.tpurcode());
        }
        
        /* validate data types... */
        TpTypesResult retTyp = null;
        
        if (output_type.equals("NULL")) {
            assertEquals(null, b_ret);
        } else {
            
            retTyp = b_ret.tptypes();
            
            assertEquals(output_type, retTyp.getType());
            assertEquals(output_sub, retTyp.getSubType());
        }
        
        if (!validate)
        {
            return;
        }
        
        /* validate the value */
        if (null!=retTyp && (AtmiConst.TPEOTYPE==tpcallerr || 
                AtmiConst.TPESVCERR==tpcallerr)) {
            
            /* New values shall be in buffer */
            if (input_type.equals("STRING")) {
                TypedString s = (TypedString)b_ret;
                assertEquals("This is unit test", s.getString());
            }
            else if (input_type.equals("JSON")) {
                TypedJson j = (TypedJson)b_ret;
                assertEquals("[]", j.getJSON());
            }
            else if (input_type.equals("CARRAY")) {
                TypedCarray c = (TypedCarray)b_ret;
                assertArrayEquals(new byte [] {9, 4, 3, 2}, c.getBytes());
            }
            else if (input_type.equals("UBF")) {
                TypedUbf u = (TypedUbf)b_ret;
                assertEquals("HELLO 11", u.BgetString(test.T_STRING_5_FLD, 11));
            }
        } else if (null!=retTyp) {
            
            /* old values shall be in buffer */
            if (output_type.equals("STRING")) {
                TypedString s = (TypedString)b_ret;
                assertEquals("HELLO FROM SERVICE", s.getString());
            }
            else if (output_type.equals("JSON")) {
                TypedJson j = (TypedJson)b_ret;
                assertEquals("{}", j.getJSON());
            }
            else if (output_type.equals("CARRAY")) {
                TypedCarray c = (TypedCarray)b_ret;
                assertArrayEquals(new byte[]{1, 2, 5, 2, 1, 2, 3, 126}, c.getBytes());
            }
            else if (output_type.equals("UBF")) {
                TypedUbf u = (TypedUbf)b_ret;
                assertEquals("HELLO UBF FROM SERVICE", u.BgetString(test.T_STRING_10_FLD, 5));
            }
        }
    }
    
    /* validate the buffers... */
    
    /**
     * This loop over the all buffer types
     * twice and will mix for each buffer the other buffer
     * @param ctx ATMI Context
     * @param syncMode run in syncrhouns mode
     */
    public void bufferCrossTestX(AtmiCtx ctx, boolean syncMode) {
        
        String [] buffers = new String[] {"NULL", "STRING", "JSON", "VIEW", 
            "UBF", "CARRAY"};
        int i, j;
        for (i=0; i<buffers.length; i++) {

            for (j=0; j<buffers.length; j++) {

                String svcnm = buffers[i].concat("RSP");
                String isub = "";
                String osub = "";

                if (buffers[j].equals("VIEW")) {
                    isub = "JVIEW1";
                }

                if (buffers[i].equals("VIEW")) {
                    osub = "JVIEW2";
                }

                /* call the server */
                testerX(ctx, svcnm, buffers[j], isub, 
                    buffers[i], osub, 0, 0, true, syncMode);

                /* validate no switch */
                int err = AtmiConst.TPEOTYPE;

                /* for views we always fail as sub-types never match..! */
                if (buffers[i].equals(buffers[j]) && !buffers[j].equals("VIEW")) {
                    err = 0;
                }

                /* types are not switched.. */
                testerX(ctx, svcnm, buffers[j], isub, 
                    buffers[j], isub, AtmiConst.TPNOCHANGE, 
                    err, true, syncMode);


                svcnm = buffers[i].concat("RSPFAIL");

                /* Check service failure, the new buffer shall be received
                 */
                testerX(ctx, svcnm, buffers[j], isub, 
                    buffers[i], osub, 0, AtmiConst.TPESVCFAIL, true, syncMode);


                svcnm = buffers[i].concat("RSPFAIL10");

                /* 
                 * Check service failure, new buffer not received.
                 */
                testerX(ctx, svcnm, buffers[j], isub, 
                    buffers[j], isub, 0, AtmiConst.TPESVCERR, true, syncMode);

            } /* for input buffer format */
        } /* for service */
    }
    
    /**
     * Cross validate 
     * @param svc Service name
     * @param input_type intput buffer type
     * @param input_sub input sub-buffer type
     * @param output_type output buffer type
     * @param output_sub output buffer sub-type
     * @param flags call flags
     * @param tpcallerr return error code/from exception
     * @return SUCCEED/FAIL
     */
    void testerConvX(AtmiCtx ctx, String svc, String input_type, String input_sub, 
        String output_type, String output_sub, long flags, int tpcallerr) {
        
        TypedBuffer b = null;
        int excpt_err = 0;
        int cd = 0;
        int rcv;
        long gotval;
        
        ctx.tplogInfo("svc: [%s] input_type: [%s] input_sub: [%s] output_type: "+
            "[%s] output_sub: [%s] flags: %d tpcallerr: %d",
            svc, input_type, input_sub, output_type, output_sub, flags, 
            tpcallerr);
        
        /* Alloc some buffer */
        b = ctx.tpalloc(input_type, input_sub, 1024);
        
        /* call the service */
        cd = ctx.tpconnect(svc, b, flags);
        rcv = 0;
        
        while (true) {
            
            TprecvResult rec = null;
            
            try {
                rec = ctx.tprecv(cd, b, flags);
            } 
            catch (AtmiException e) {
                excpt_err = e.getErrorCode();
            }
            
            assertEquals(tpcallerr, excpt_err);
            
            if (excpt_err > 0) {
                
                /* nothing todo.. */
                ctx.tpdiscon(cd);
                return;
            }
            
            b = rec.getBuffer();
            
            /* validate the response buffer */
            TpTypesResult retTyp;
        
            if (output_type.equals("NULL")) {
                assertEquals(null, b);
            } else {

                retTyp = b.tptypes();
                assertEquals(output_type, retTyp.getType());
                assertEquals(output_sub, retTyp.getSubType());
            }

            if (output_type.equals("STRING")) {
                TypedString s = (TypedString)b;
                assertNotNull(s);
                assertNotNull(s.getString());
                gotval = Integer.parseInt(s.getString());
            }
            else if (output_type.equals("JSON")) {
                /* it is the same string... */
                TypedJson j = (TypedJson)b;
                gotval = Integer.parseInt(j.getJSON());
            }
            else if (output_type.equals("CARRAY")) {
                TypedCarray c = (TypedCarray)b;
                gotval = c.getBytes()[0];
            }
            else if (output_type.equals("UBF")) {
                TypedUbf u = (TypedUbf)b;
                gotval = u.BgetLong(test.T_LONG_FLD, 0);
            }
            else if (output_type.equals("VIEW")) {
                gotval = rcv + 3;
            }
            else if (output_type.equals("NULL")) {
                gotval = rcv + 3;
            }
            else {
                gotval = AtmiConst.FAIL;
            }
            
            assertEquals(rcv + 3, gotval);
            
            rcv++;
            
            if (rec.getRevent() != 0)
            {
                assertEquals(AtmiConst.TPEV_SENDONLY, rec.getRevent());
                break;
            }
            
        }

        assertEquals(100, rcv);

        /* Now send some stuff */
        
        b = ctx.tpalloc(input_type, input_sub, 1024);
        
        for (int i=0; i<100; i++)
        {
            int snd = i + 7;
            if (input_type.equals("STRING")) {
                TypedString s = (TypedString)b;
                s.setString(Integer.toString(snd));
            }
            else if (input_type.equals("JSON")) {
                /* it is the same string... */
                TypedJson j = (TypedJson)b;
                
                /* I know it is not a json, but basically it is string transport... */
                j.setJSON(Integer.toString(snd));
            }
            else if (input_type.equals("CARRAY")) {
                TypedCarray c = (TypedCarray)b;
                
                byte bb = (byte)snd;
                c.setBytes(new byte[] {bb});
            }
            else if (input_type.equals("UBF")) {
                TypedUbf u = (TypedUbf)b;
                u.Bchg(test.T_LONG_FLD, 0, snd);
            }
            else if (input_type.equals("VIEW")) {
                /* nothing to send.. */
            }
            else if (input_type.equals("NULL")) {
                /* nothing to send.. */
            }
            
            assertEquals(0, ctx.tpsend(cd, b, flags));
        }
        
        /* disconnect from conv */
        ctx.tpdiscon(cd);
        
    }
    
    /**
     * Test conversation mode buffer cross switching
     * @param ctx ATMI Context
     */
    public void bufferCrossConvTestX(AtmiCtx ctx) {
        
        String [] buffers = new String[] {"NULL", "STRING", "JSON", "VIEW", 
            "UBF", "CARRAY"};
        int i, j;
        for (i=0; i<buffers.length; i++) {

            for (j=0; j<buffers.length; j++) {

                String svcnm = buffers[i].concat("CONV");
                String isub = "";
                String osub = "";

                if (buffers[j].equals("VIEW")) {
                    isub = "JVIEW1";
                }

                if (buffers[i].equals("VIEW")) {
                    osub = "JVIEW2";
                }
                
                testerConvX(ctx, svcnm, buffers[j], isub, 
                     buffers[i], osub, 0, 0);
        

                /* call the server */
                testerConvX(ctx, svcnm, buffers[j], isub, 
                    buffers[i], osub, 0, 0);

                /* validate no switch */
                int err = AtmiConst.TPEOTYPE;

                /* for views we always fail as sub-types never match..! */
                if (buffers[i].equals(buffers[j]) && !buffers[j].equals("VIEW")) {
                    err = 0;
                }

                /* types are not switched.. */
                testerConvX(ctx, svcnm, buffers[j], isub, 
                    buffers[j], isub, AtmiConst.TPNOCHANGE, 
                    err);

            } /* for input buffer format */
        } /* for service */
    }
    
    /**
     * Test test buffer
     * @param ctx Atmi Context
     * @param input_type buffer type
     * @param input_sub sub-type
     * @param testid test value
     * @return allocated buffer
     */
    TypedBuffer getTestBuffer(AtmiCtx ctx, String input_type, String input_sub, 
            int testid) {
        
        TypedBuffer b = null;
        
        ctx.tplogInfo("Test buffer type: [%s] sub_type: [%s] id: %d",
            input_type, input_sub, testid);
        
        if (input_type.equals("NULL")) {
            b = null;
        } 
        else if (input_type.equals("STRING")) {
            b = ctx.newString(String.format("This is unit test %d", testid));
        }
        else if (input_type.equals("JSON")) {
            b = ctx.newJson(String.format("[\"test\":%d]", testid));
        }
        else if (input_type.equals("CARRAY")) {
            b = ctx.newCarray(new byte [] {9, 4, 3, (byte)testid});
        }
        else if (input_type.equals("VIEW")) {
            b = ctx.tpalloc(input_type, input_sub, 0);
        }
        else if (input_type.equals("UBF")) {
            TypedUbf u;
            b = ctx.tpalloc(input_type, "", 0);
            
            u = (TypedUbf)b;   
            u.Bchg(test.T_STRING_5_FLD, 11, String.format("HELLO %d", testid));
        }
        
        return b;
    }
    
    /**
     * Test the ATMI buffer to match value generated by getTestBuffer()
     * @param ctx ATMI Context
     * @param input_type input buffer type
     * @param input_sub input sub-type code, atmi
     * @param testid test number
     */
    void testBuffer(AtmiCtx ctx, String input_type, String input_sub, 
            TypedBuffer b_ret, int testid) {
        
        ctx.tplogInfo("Validate test buffer type: [%s] sub_type: [%s] id: %d",
            input_type, input_sub, testid);
        
       /* New values shall be in buffer */
	if (input_type.equals("STRING")) {
            TypedString s = (TypedString)b_ret;
            String comp = String.format("This is unit test %d", testid);
            assertEquals(comp, s.getString());
	}
	else if (input_type.equals("JSON")) {
            TypedJson j = (TypedJson)b_ret;
            assertEquals(String.format("[\"test\":%d]", testid), j.getJSON());
	}
	else if (input_type.equals("CARRAY")) {
            TypedCarray c = (TypedCarray)b_ret;
            assertArrayEquals(new byte [] {9, 4, 3, (byte)testid}, c.getBytes());
	}
	else if (input_type.equals("UBF")) {
            TypedUbf u = (TypedUbf)b_ret;
            assertEquals(String.format("HELLO %d", testid), 
                    u.BgetString(test.T_STRING_5_FLD, 11));
	}
        else if (input_type.equals("NULL")) {
            assertEquals(null, b_ret);
	}
        else if (input_type.equals("VIEW")) {
            /* assertEquals(null, b_ret); */
            /* Check view type (currently ...) */
            TypedView v = (TypedView)b_ret;
            
            TpTypesResult typ = v.tptypes();
            
            assertEquals(typ.getType(), input_type);
            assertEquals(typ.getSubType(), input_sub);
	}
    }
    
    
}
