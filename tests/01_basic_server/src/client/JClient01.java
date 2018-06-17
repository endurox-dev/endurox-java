import org.endurox.*;
import java.util.*;

public class JClient01 {

    public static void main(String[] args) {
    
        AtmiCtx ctx = new AtmiCtx();
        ctx.tpInit(null);
        ctx.tpLogInfo("Client process started...");
        ctx.userLog("Hello from Userlog");
    }
}
