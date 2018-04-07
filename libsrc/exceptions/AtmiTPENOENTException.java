package org.endurox;

/**
 * TPENOENT exception
 */
public class AtmiTPENOENTException extends AtmiException {
        
    public AtmiTPENOENTException(String msg) {
        super(msg, AtmiConstants.TPENOENT);
    }
    
    public AtmiTPENOENTException(String s, Throwable throwable) {
        super(s, AtmiConstants.TPENOENT, throwable);
    }
}


