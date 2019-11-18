import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import org.junit.Test;
import static org.junit.Assert.*;
import org.endurox.*;
import org.endurox.exceptions.AtmiTPEINVALException;
import org.endurox.exceptions.AtmiTPENOENTException;

/**
 * Perform logging framework testing..
 */
public class LoggerTest {
    
    
    /**
     * Check that file contains particular text sequences
     * @param filenm filename to check
     * @param search value to search for
     * @param mustContain if set to true, file shall contain the message
     *  if false, then it must not contain.
     */
    public void chkFile(String filenm, String search, boolean mustContain)
    {  
        StringBuffer sb=new StringBuffer();
        
        try  
        {  
            File file=new File(filenm);
            FileReader fr=new FileReader(file);
            BufferedReader br=new BufferedReader(fr);
            String line;
            while((line=br.readLine())!=null)  
            {  
                sb.append(line);
            }
            fr.close();
        }  
        catch(IOException e)  
        {  
            e.printStackTrace();  
        }
        
        String strChk = sb.toString();
        
        if (mustContain) {
            assertEquals(true, strChk.contains(search));
        } else {
            assertEquals(false, strChk.contains(search));
        }
    }
    
    @Test
    public void testTplogsetreqfile() {
        
        for (int i=0; i<1000; i++)
        {
            AtmiCtx ctx = new AtmiCtx();
            assertNotEquals(ctx.getCtx(), 0x0);
            TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
            
            // set request file
            String reqFile = String.format("./log/request.%d", i);
            
            // set the request file
            ctx.tplogsetreqfile(ub, reqFile, null);
            ctx.tplogError("REQLOG1");
            
            // Close request file
            ctx.tplogclosereqfile();
            ctx.tplogError("PROCLOG1");
            
            //Try again from buffer field..
            ctx.tplogsetreqfile(ub, null, null);
            ctx.tplogError("REQLOG2");
            //Get the name from buffer
            String bufFile=ctx.tploggetbufreqfile(ub);
            assertEquals(reqFile, bufFile);
            
            String ctxFile=ctx.tploggetreqfile();
            assertEquals(reqFile, ctxFile);
            ctx.tplogclosereqfile();
            
            //Can close twice, no problem...
            ctx.tplogclosereqfile();
            
            
            ctx.tplogdelbufreqfile(ub);
            boolean gotex = false;
            try {
                
                ctx.tploggetbufreqfile(ub);
            } catch ( AtmiTPENOENTException e) {
                gotex = true;
            }
            
            assertEquals(true, gotex);
            
            
            ctx.tplogError("PROCLOG2");
            //Get the name from the ATMI Context
            
            //Not set
            ctxFile=ctx.tploggetreqfile();
            assertEquals(null, ctxFile);
            
            
            ctx.cleanup();
            ub.cleanup();
        }
        
        chkFile("./log/request.400", "REQLOG1", true);
        chkFile("./log/request.400", "REQLOG2", true);
        
        chkFile("./log/request.400", "PROCLOG1", false);
        chkFile("./log/request.400", "PROCLOG2", false);
        
        
        chkFile("./log/request.0", "REQLOG1", true);
        chkFile("./log/request.0", "REQLOG2", true);
        
        chkFile("./log/request.0", "PROCLOG1", false);
        chkFile("./log/request.0", "PROCLOG2", false);
        
    }
    
    @Test
    public void testTplogsetreqfile_direct() {
        
        for (int i=2000; i<3000; i++)
        {
            AtmiCtx ctx = new AtmiCtx();
            assertNotEquals(ctx.getCtx(), 0x0);
            TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
            
            // set request file
            String reqFile = String.format("./log/request.%d", i);
            
            // set the request file
            ctx.tplogsetreqfile_direct(reqFile);
            ctx.tplogError("REQLOG1");
            
            // Close request file
            ctx.tplogclosereqfile();
            ctx.tplogError("PROCLOG1");
            
            ctx.cleanup();
            ub.cleanup();
        }
        
        chkFile("./log/request.2000", "REQLOG1", true);
        chkFile("./log/request.2000", "PROCLOG1", false);
        
        chkFile("./log/request.2999", "REQLOG1", true);
        chkFile("./log/request.2999", "PROCLOG1", false);
    }
    
    @Test
    public void testTplogconfig() {
        
        for (int i=3000; i<4000; i++)
        {
            AtmiCtx ctx = new AtmiCtx();
            assertNotEquals(ctx.getCtx(), 0x0);

            ctx.tplogconfig(AtmiConst.LOG_FACILITY_TP, AtmiConst.LOG_DEBUG, 
                    null, null, null);

            
            String reqFile = String.format("./log/request.%d", i);
            
            String reqFileA = String.format("./log/request.%dA", i);
            
            String debugLine = String.format("file=%s", reqFile);
            
            //Open logconfig
            ctx.tplogconfig(AtmiConst.LOG_FACILITY_TP_REQUEST, AtmiConst.LOG_WARN, 
                    debugLine, "HELO", null);
            
            //Write something
            ctx.tplogDebug("DEBUG_NOT_LOGGED");
            ctx.tplogWarn("WARN_LOGGED");
            assertEquals(reqFile, ctx.tploggetreqfile());
            
            ctx.tplogclosereqfile();
            assertEquals(null, ctx.tploggetreqfile());
            
            ctx.tplogconfig(AtmiConst.LOG_FACILITY_TP_REQUEST, AtmiConst.LOG_WARN, 
                    null, "HELO", reqFileA);
            
            ctx.tplogWarn("WARN_LOGGED2");
            ctx.tplogclosereqfile();
        }
        
        //Check the data
        chkFile("./log/request.3011", "DEBUG_NOT_LOGGED", false);
        chkFile("./log/request.3011", "WARN_LOGGED", true);
        chkFile("./log/request.3011", "HELO", true);
        chkFile("./log/request.3011A", "WARN_LOGGED2", true);
        
    }
    
    /**
     * Check invalid arguments exception
     */
    @Test(expected = AtmiTPEINVALException.class)
    public void TestTplogsetreqfileInvalParam() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
            
        ctx.tplogsetreqfile(null, null, null);
        
        ctx.cleanup();
                    
    }
    
    /**
     * Check null argument exception
     */
    @Test(expected = NullPointerException.class)
    public void TestTploggetbufreqfileNullBuf() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
            
        ctx.tploggetbufreqfile(null);
        
        ctx.cleanup();
    }
    
}
