package org.endurox;

/**
 * TPEOS exception
 */
public class AtmiTPEOSException extends AtmiException {
        
    public AtmiTPEOSException(String msg) {
        super(msg, AtmiConstants.TPEOS);
    }
    
    public AtmiTPEOSException(String s, Throwable throwable) {
        super(s, AtmiConstants.TPEOS, throwable);
    }
}


