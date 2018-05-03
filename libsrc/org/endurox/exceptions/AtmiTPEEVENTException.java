package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * TPEEVENT exception
 */
public class AtmiTPEEVENTException extends AtmiException {
        
    public AtmiTPEEVENTException(String msg) {
        super(AtmiConstants.TPEEVENT, msg);
    }
    
    public AtmiTPEEVENTException(String msg, Throwable throwable) {
        super(AtmiConstants.TPEEVENT, msg, throwable);
    }
}


