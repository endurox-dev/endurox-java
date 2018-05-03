package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * TPEHAZARD exception
 */
public class AtmiTPEHAZARDException extends AtmiException {
        
    public AtmiTPEHAZARDException(String msg) {
        super(AtmiConstants.TPEHAZARD, msg);
    }
    
    public AtmiTPEHAZARDException(String msg, Throwable throwable) {
        super(AtmiConstants.TPEHAZARD, msg, throwable);
    }
}


