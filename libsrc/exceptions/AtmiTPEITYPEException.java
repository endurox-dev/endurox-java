package org.endurox;

/**
 * TPEITYPE exception
 */
public class AtmiTPEITYPEException extends AtmiException {
        
    public AtmiTPEITYPEException(String msg) {
        super(AtmiConstants.TPEITYPE, msg);
    }
    
    public AtmiTPEITYPEException(String msg, Throwable throwable) {
        super(AtmiConstants.TPEITYPE, msg, throwable);
    }
}


