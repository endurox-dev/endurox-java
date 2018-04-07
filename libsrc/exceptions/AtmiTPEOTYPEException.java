package org.endurox;

/**
 * TPEOTYPE exception
 */
public class AtmiTPEOTYPEException extends AtmiException {
        
    public AtmiTPEOTYPEException(String msg) {
        super(msg, AtmiConstants.TPEOTYPE);
    }
    
    public AtmiTPEOTYPEException(String s, Throwable throwable) {
        super(s, AtmiConstants.TPEOTYPE, throwable);
    }
}


