package org.endurox;

/**
 * TPEMATCH exception
 */
public class AtmiTPEMATCHException extends AtmiException {
        
    public AtmiTPEMATCHException(String msg) {
        super(msg, AtmiConstants.TPEMATCH);
    }
    
    public AtmiTPEMATCHException(String s, Throwable throwable) {
        super(s, AtmiConstants.TPEMATCH, throwable);
    }
}


