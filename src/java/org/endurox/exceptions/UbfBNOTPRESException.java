package org.endurox.exceptions;
import org.endurox.AtmiConstants;

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


