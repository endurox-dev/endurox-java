package org.endurox;

/**
 * BEINVAL exception
 */
public class UbfBEINVALException extends UbfException {
        
    public UbfBEINVALException(String msg) {
        super(msg, AtmiConstants.BEINVAL);
    }
    
    public UbfBEINVALException(String s, Throwable throwable) {
        super(s, AtmiConstants.BEINVAL, throwable);
    }
}


