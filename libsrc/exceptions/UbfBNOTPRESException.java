package org.endurox;

/**
 * BNOTPRES exception
 */
public class UbfBNOTPRESException extends UbfException {
        
    public UbfBNOTPRESException(String msg) {
        super(AtmiConstants.BNOTPRES, msg);
    }
    
    public UbfBNOTPRESException(String msg, Throwable throwable) {
        super(AtmiConstants.BNOTPRES, msg, throwable);
    }
}


