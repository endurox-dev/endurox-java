import org.endurox.*;

/**
 * Basic client process, calls service
 * @author mvitolin
 */
public class JClient01 {

    public static void main(String[] args) {
    
        AtmiCtx ctx = new AtmiCtx();
        ctx.tpinit(null);
        ctx.tplogInfo("Client process started...");
        ctx.userlog("Hello from Userlog");

        TypedUBF u = (TypedUBF) ctx.tpalloc("UBF", null, 1024);
        try
        {
            u = (TypedUBF)ctx.tpcall("HELLOSVC", u, 0);
        }
        catch (AtmiException e)
        {
            ctx.tplogInfo("got exception !!!: %s", e.toString());
            System.exit(AtmiConstants.FAIL);
        }
        
        ctx.tplogInfo("Test OK...");
        
        /* Print the UBF buffer? */
        
        System.exit(AtmiConstants.SUCCEED);
    }
}
