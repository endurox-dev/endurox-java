package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * TPEABORT exception
 */
public class AtmiTPEABORTException extends AtmiException {
        
    public AtmiTPEABORTException(String msg) {
        super(AtmiConst.TPEABORT, msg);
    }
    
    public AtmiTPEABORTException(String msg, Throwable throwable) {
        super(AtmiConst.TPEABORT, msg, throwable);
    }
}


