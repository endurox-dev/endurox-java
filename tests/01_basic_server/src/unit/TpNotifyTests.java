import static org.junit.Assert.*;
import org.endurox.*;
import org.endurox.exceptions.AtmiException;

/**
 * Perform tpnotify tests
 */
public class TpNotifyTests implements UnsolCallback {
             
    int nrstrings = 0 ;
    int nrjson = 0;
    int nrubf = 0 ;
    int nrcarrays = 0;
    int nrnull = 0;
    int nrview = 0;
    
    /**
     * Receive different types of messages
     * @param ctx Atmi context
     * @param buf typed buffer received
     * @param flags flags
     */
    public void unsolCallback(AtmiCtx ctx, TypedBuffer buf, long flags)
    {
        TpTypesResult t = buf.tptypes();
        
        if (t.getType().equals("STRING")) {
            nrstrings++;
        } else if (t.getType().equals("JSON")) {
            nrjson++;
        } else if (t.getType().equals("UBF")) {
            nrubf++;
        }
    }
    
}
