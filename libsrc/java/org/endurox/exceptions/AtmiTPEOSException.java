package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * TPEOS exception
 */
public class AtmiTPEOSException extends AtmiException {
        
    public AtmiTPEOSException(String msg) {
        super(AtmiConst.TPEOS, msg);
    }
    
    public AtmiTPEOSException(String msg, Throwable throwable) {
        super(AtmiConst.TPEOS, msg, throwable);
    }
}


