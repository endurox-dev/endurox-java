package org.endurox;

/**
 * TPESVCFAIL exception
 */
public class AtmiTPESVCFAILException extends AtmiException {
        
    public AtmiTPESVCFAILException(String msg) {
        super(msg, AtmiConstants.TPESVCFAIL);
    }
    
    public AtmiTPESVCFAILException(String s, Throwable throwable) {
        super(s, AtmiConstants.TPESVCFAIL, throwable);
    }
}


