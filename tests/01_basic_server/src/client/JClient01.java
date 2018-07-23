import org.endurox.AtmiConstants;
import org.endurox.AtmiCtx;
import org.endurox.TypedUBF;
import org.endurox.exceptions.AtmiException;
import org.endurox.exceptions.AtmiTPENOENTException;

/**
 * Basic client process, calls service
 * @author mvitolin
 */
public class JClient01 {

    public static void main(String[] args) {
    
        boolean got_noent = false;
        int ret = AtmiConstants.TPSUCCESS;
        AtmiCtx ctx = new AtmiCtx();
        ctx.tpinit(null);
        ctx.tplogInfo("Client process started...");
        ctx.userlog("Hello from Userlog");

        TypedUBF u = (TypedUBF) ctx.tpalloc("UBF", null, 1024);
        
        /* set some fields in buffer */
        
        try
        {
            u = (TypedUBF)ctx.tpcall("NOSVC", u, 0);
        }
        catch (AtmiTPENOENTException e)
        {
            got_noent = true;
            ctx.tplogInfo("got exception !!!: %s", e.toString());
            u = (TypedUBF)e.getReplyBuffer();
        }
        if (!got_noent)
        {
            ctx.tplogError("Expected AtmiTPENOENTException but wasn't");
            ret = AtmiConstants.TPFAIL;
        }
        
        if (AtmiConstants.TPSUCCESS==ret)
        {
            ctx.tplogInfo("Test OK...");
            /* Print the UBF buffer? */
            System.exit(AtmiConstants.SUCCEED);
        }
        else
        {
            ctx.tplogInfo("Test Failed...");
            /* Print the UBF buffer? */
            System.exit(AtmiConstants.TPFAIL);
        }
    }
}
