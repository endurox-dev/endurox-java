package org.endurox;

/**
 * TPINITFAIL exception
 */
public class AtmiTPINITFAILException extends AtmiException {
        
    public AtmiTPINITFAILException(String msg) {
        super(msg, AtmiConstants.TPINITFAIL);
    }
    
    public AtmiTPINITFAILException(String s, Throwable throwable) {
        super(s, AtmiConstants.TPINITFAIL, throwable);
    }
}


