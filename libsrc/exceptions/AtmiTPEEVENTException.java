package org.endurox;

/**
 * TPEEVENT exception
 */
public class AtmiTPEEVENTException extends AtmiException {
        
    public AtmiTPEEVENTException(String msg) {
        super(msg, AtmiConstants.TPEEVENT);
    }
    
    public AtmiTPEEVENTException(String s, Throwable throwable) {
        super(s, AtmiConstants.TPEEVENT, throwable);
    }
}


