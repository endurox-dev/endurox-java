package org.endurox;

/**
 * TPERMERR exception
 */
public class AtmiTPERMERRException extends AtmiException {
        
    public AtmiTPERMERRException(String msg) {
        super(msg, AtmiConstants.TPERMERR);
    }
    
    public AtmiTPERMERRException(String s, Throwable throwable) {
        super(s, AtmiConstants.TPERMERR, throwable);
    }
}


