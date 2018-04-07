package org.endurox;

/**
 * TPEITYPE exception
 */
public class AtmiTPEITYPEException extends AtmiException {
        
    public AtmiTPEITYPEException(String msg) {
        super(msg, AtmiConstants.TPEITYPE);
    }
    
    public AtmiTPEITYPEException(String s, Throwable throwable) {
        super(s, AtmiConstants.TPEITYPE, throwable);
    }
}


