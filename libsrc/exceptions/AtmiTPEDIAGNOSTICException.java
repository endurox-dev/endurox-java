package org.endurox;

/**
 * TPEDIAGNOSTIC exception
 */
public class AtmiTPEDIAGNOSTICException extends AtmiException {
        
    public AtmiTPEDIAGNOSTICException(String msg) {
        super(msg, AtmiConstants.TPEDIAGNOSTIC);
    }
    
    public AtmiTPEDIAGNOSTICException(String s, Throwable throwable) {
        super(s, AtmiConstants.TPEDIAGNOSTIC, throwable);
    }
}


