package org.endurox;

/**
 * TPEPERM exception
 */
public class AtmiTPEPERMException extends AtmiException {
        
    public AtmiTPEPERMException(String msg) {
        super(msg, AtmiConstants.TPEPERM);
    }
    
    public AtmiTPEPERMException(String s, Throwable throwable) {
        super(s, AtmiConstants.TPEPERM, throwable);
    }
}


