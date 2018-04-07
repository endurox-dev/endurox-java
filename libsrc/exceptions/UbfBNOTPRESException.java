package org.endurox;

/**
 * BNOTPRES exception
 */
public class UbfBNOTPRESException extends UbfException {
        
    public UbfBNOTPRESException(String msg) {
        super(msg, AtmiConstants.BNOTPRES);
    }
    
    public UbfBNOTPRESException(String s, Throwable throwable) {
        super(s, AtmiConstants.BNOTPRES, throwable);
    }
}


