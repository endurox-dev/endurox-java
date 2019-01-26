package org.endurox.exceptions;
import org.endurox.AtmiConst;
        
/**
 * TPEDIAGNOSTIC exception
 */
public class AtmiTPEDIAGNOSTICException extends AtmiException {
        
    public AtmiTPEDIAGNOSTICException(String msg) {
        super(AtmiConst.TPEDIAGNOSTIC, msg);
    }
    
    public AtmiTPEDIAGNOSTICException(String msg, Throwable throwable) {
        super(AtmiConst.TPEDIAGNOSTIC, msg, throwable);
    }
}


