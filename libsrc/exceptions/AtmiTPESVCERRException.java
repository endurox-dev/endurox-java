package org.endurox;

/**
 * TPESVCERR exception
 */
public class AtmiTPESVCERRException extends AtmiException {
        
    public AtmiTPESVCERRException(String msg) {
        super(AtmiConstants.TPESVCERR, msg);
    }
    
    public AtmiTPESVCERRException(String msg, Throwable throwable) {
        super(AtmiConstants.TPESVCERR, msg, throwable);
    }
}


