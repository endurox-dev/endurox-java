package org.endurox.exceptions;
import org.endurox.AtmiConstants;
        
/**
 * TPEDIAGNOSTIC exception
 */
public class AtmiTPEDIAGNOSTICException extends AtmiException {
        
    public AtmiTPEDIAGNOSTICException(String msg) {
        super(AtmiConstants.TPEDIAGNOSTIC, msg);
    }
    
    public AtmiTPEDIAGNOSTICException(String msg, Throwable throwable) {
        super(AtmiConstants.TPEDIAGNOSTIC, msg, throwable);
    }
}


