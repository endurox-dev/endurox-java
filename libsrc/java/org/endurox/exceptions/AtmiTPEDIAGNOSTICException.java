package org.endurox.exceptions;
import org.endurox.AtmiConst;
import org.endurox.TPQCTL;
        
/**
 * TPEDIAGNOSTIC exception
 */
public class AtmiTPEDIAGNOSTICException extends AtmiException {
    
    
    TPQCTL qctl;

    /**
     * Get queue control object pass to tpenqueue(3) or tpdequeue(3)
     * @return Queue control structure
     */
    public TPQCTL getQctl() {
        return qctl;
    }
        
    public AtmiTPEDIAGNOSTICException(String msg) {
        super(AtmiConst.TPEDIAGNOSTIC, msg);
    }
    
    public AtmiTPEDIAGNOSTICException(String msg, Throwable throwable) {
        super(AtmiConst.TPEDIAGNOSTIC, msg, throwable);
    }
}


