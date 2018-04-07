package org.endurox;

/**
 * TPESYSTEM exception
 */
public class AtmiTPESYSTEMException extends AtmiException {
        
    public AtmiTPESYSTEMException(String msg) {
        super(msg, AtmiConstants.TPESYSTEM);
    }
    
    public AtmiTPESYSTEMException(String s, Throwable throwable) {
        super(s, AtmiConstants.TPESYSTEM, throwable);
    }
}


