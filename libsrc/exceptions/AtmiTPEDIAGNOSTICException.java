package org.endurox;

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


