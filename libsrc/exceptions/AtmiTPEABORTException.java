package org.endurox;

/**
 * TPEABORT exception
 */
public class AtmiTPEABORTException extends AtmiException {
        
    public AtmiTPEABORTException(String msg) {
        super(AtmiConstants.TPEABORT, msg);
    }
    
    public AtmiTPEABORTException(String msg, Throwable throwable) {
        super(AtmiConstants.TPEABORT, msg, throwable);
    }
}


