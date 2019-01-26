package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * TPEHAZARD exception
 */
public class AtmiTPEHAZARDException extends AtmiException {
        
    public AtmiTPEHAZARDException(String msg) {
        super(AtmiConst.TPEHAZARD, msg);
    }
    
    public AtmiTPEHAZARDException(String msg, Throwable throwable) {
        super(AtmiConst.TPEHAZARD, msg, throwable);
    }
}


