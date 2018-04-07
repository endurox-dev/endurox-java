package org.endurox;

/**
 * TPERELEASE exception
 */
public class AtmiTPERELEASEException extends AtmiException {
        
    public AtmiTPERELEASEException(String msg) {
        super(msg, AtmiConstants.TPERELEASE);
    }
    
    public AtmiTPERELEASEException(String s, Throwable throwable) {
        super(s, AtmiConstants.TPERELEASE, throwable);
    }
}


