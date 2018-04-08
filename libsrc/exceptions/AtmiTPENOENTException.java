package org.endurox;

/**
 * TPENOENT exception
 */
public class AtmiTPENOENTException extends AtmiException {
        
    public AtmiTPENOENTException(String msg) {
        super(AtmiConstants.TPENOENT, msg);
    }
    
    public AtmiTPENOENTException(String msg, Throwable throwable) {
        super(AtmiConstants.TPENOENT, msg, throwable);
    }
}


