package org.endurox;

/**
 * TPEPROTO exception
 */
public class AtmiTPEPROTOException extends AtmiException {
        
    public AtmiTPEPROTOException(String msg) {
        super(msg, AtmiConstants.TPEPROTO);
    }
    
    public AtmiTPEPROTOException(String s, Throwable throwable) {
        super(s, AtmiConstants.TPEPROTO, throwable);
    }
}


