package org.endurox;

/**
 * TPEBADDESC exception
 */
public class AtmiTPEBADDESCException extends AtmiException {
        
    public AtmiTPEBADDESCException(String msg) {
        super(msg, AtmiConstants.TPEBADDESC);
    }
    
    public AtmiTPEBADDESCException(String s, Throwable throwable) {
        super(s, AtmiConstants.TPEBADDESC, throwable);
    }
}


