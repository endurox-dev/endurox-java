package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * TPEEVENT exception
 */
public class AtmiTPEEVENTException extends AtmiException {
        
    public AtmiTPEEVENTException(String msg) {
        super(AtmiConst.TPEEVENT, msg);
    }
    
    public AtmiTPEEVENTException(String msg, Throwable throwable) {
        super(AtmiConst.TPEEVENT, msg, throwable);
    }
}


