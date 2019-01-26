import org.endurox.AtmiConst;
import org.endurox.AtmiCtx;
import org.endurox.TypedUbf;
import org.endurox.exceptions.AtmiException;
import org.endurox.exceptions.AtmiTPENOENTException;

/**
 * Basic client process, calls service
 * @author mvitolin
 */
public class JClient01 {

    public static void main(String[] args) {
    
        boolean got_noent = false;
        int ret = AtmiConst.TPSUCCESS;
        int i;
        
        AtmiCtx ctx = new AtmiCtx();
        ctx.tpinit(null);
        ctx.tplogInfo("Client process started...");
        ctx.userlog("Hello from Userlog");
        
        for (i=0; i<1000; i++)
        {
            TypedUbf u = (TypedUbf) ctx.tpalloc("UBF", null, 1024);

            /* set some fields in buffer */
            try
            {
                u = (TypedUbf)ctx.tpcall("NOSVC", u, 0);
            }
            catch (AtmiTPENOENTException e)
            {
                got_noent = true;
                ctx.tplogInfo("got exception !!!: %s", e.toString());
                u = (TypedUbf)e.getReplyBuffer();
            }

            if (!got_noent)
            {
                ctx.tplogError("Expected AtmiTPENOENTException but wasn't");
                ret = AtmiConst.TPFAIL;
            }

            if (AtmiConst.TPSUCCESS!=ret)
            {
                ctx.tplogInfo("Test Failed...");
                /* Print the UBF buffer? */
                System.exit(AtmiConst.TPFAIL);
            }
        }
        
        System.exit(AtmiConst.SUCCEED);
    }
}
