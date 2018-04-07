package org.endurox;

/**
 * TPETRAN exception
 */
public class AtmiTPETRANException extends AtmiException {
        
    public AtmiTPETRANException(String msg) {
        super(msg, AtmiConstants.TPETRAN);
    }
    
    public AtmiTPETRANException(String s, Throwable throwable) {
        super(s, AtmiConstants.TPETRAN, throwable);
    }
}


