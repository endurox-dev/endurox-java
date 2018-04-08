package org.endurox;

/**
 * BEINVAL exception
 */
public class UbfBEINVALException extends UbfException {
        
    public UbfBEINVALException(String msg) {
        super(AtmiConstants.BEINVAL, msg);
    }
    
    public UbfBEINVALException(String msg, Throwable throwable) {
        super(AtmiConstants.BEINVAL, msg, throwable);
    }
}


