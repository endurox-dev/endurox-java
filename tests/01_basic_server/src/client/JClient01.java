import org.endurox.*;
import java.util.*;

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
                ctx.tplogInfo("got exception !!!");
        }
    }
}
