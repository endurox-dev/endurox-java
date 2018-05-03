package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * TPEOS exception
 */
public class AtmiTPEOSException extends AtmiException {
        
    public AtmiTPEOSException(String msg) {
        super(AtmiConstants.TPEOS, msg);
    }
    
    public AtmiTPEOSException(String msg, Throwable throwable) {
        super(AtmiConstants.TPEOS, msg, throwable);
    }
}


