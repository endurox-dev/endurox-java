package org.endurox;

/**
 * TPESVCFAIL exception
 */
public class AtmiTPESVCFAILException extends AtmiException {
        
    public AtmiTPESVCFAILException(String msg) {
        super(AtmiConstants.TPESVCFAIL, msg);
    }
    
    public AtmiTPESVCFAILException(String msg, Throwable throwable) {
        super(AtmiConstants.TPESVCFAIL, msg, throwable);
    }
}


