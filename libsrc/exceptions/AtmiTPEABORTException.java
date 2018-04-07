package org.endurox;

/**
 * TPEABORT exception
 */
public class AtmiTPEABORTException extends AtmiException {
        
    public AtmiTPEABORTException(String msg) {
        super(msg, AtmiConstants.TPEABORT);
    }
    
    public AtmiTPEABORTException(String s, Throwable throwable) {
        super(s, AtmiConstants.TPEABORT, throwable);
    }
}


