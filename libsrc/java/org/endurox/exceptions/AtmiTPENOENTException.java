package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * TPENOENT exception
 */
public class AtmiTPENOENTException extends AtmiException {
        
    public AtmiTPENOENTException(String msg) {
        super(AtmiConst.TPENOENT, msg);
    }
    
    public AtmiTPENOENTException(String msg, Throwable throwable) {
        super(AtmiConst.TPENOENT, msg, throwable);
    }
}


