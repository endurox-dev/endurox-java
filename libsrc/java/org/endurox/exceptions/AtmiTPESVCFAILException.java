package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * TPESVCFAIL exception
 */
public class AtmiTPESVCFAILException extends AtmiException {
        
    public AtmiTPESVCFAILException(String msg) {
        super(AtmiConst.TPESVCFAIL, msg);
    }
    
    public AtmiTPESVCFAILException(String msg, Throwable throwable) {
        super(AtmiConst.TPESVCFAIL, msg, throwable);
    }
}


