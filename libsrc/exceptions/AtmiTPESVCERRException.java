package org.endurox;

/**
 * TPESVCERR exception
 */
public class AtmiTPESVCERRException extends AtmiException {
        
    public AtmiTPESVCERRException(String msg) {
        super(msg, AtmiConstants.TPESVCERR);
    }
    
    public AtmiTPESVCERRException(String s, Throwable throwable) {
        super(s, AtmiConstants.TPESVCERR, throwable);
    }
}


