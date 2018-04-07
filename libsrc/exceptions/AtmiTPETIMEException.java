package org.endurox;

/**
 * TPETIME exception
 */
public class AtmiTPETIMEException extends AtmiException {
        
    public AtmiTPETIMEException(String msg) {
        super(msg, AtmiConstants.TPETIME);
    }
    
    public AtmiTPETIMEException(String s, Throwable throwable) {
        super(s, AtmiConstants.TPETIME, throwable);
    }
}


