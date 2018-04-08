package org.endurox;

/**
 * TPEMATCH exception
 */
public class AtmiTPEMATCHException extends AtmiException {
        
    public AtmiTPEMATCHException(String msg) {
        super(AtmiConstants.TPEMATCH, msg);
    }
    
    public AtmiTPEMATCHException(String msg, Throwable throwable) {
        super(AtmiConstants.TPEMATCH, msg, throwable);
    }
}


