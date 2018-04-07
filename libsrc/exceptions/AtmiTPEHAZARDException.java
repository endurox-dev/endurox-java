package org.endurox;

/**
 * TPEHAZARD exception
 */
public class AtmiTPEHAZARDException extends AtmiException {
        
    public AtmiTPEHAZARDException(String msg) {
        super(msg, AtmiConstants.TPEHAZARD);
    }
    
    public AtmiTPEHAZARDException(String s, Throwable throwable) {
        super(s, AtmiConstants.TPEHAZARD, throwable);
    }
}


