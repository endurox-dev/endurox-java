package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * BNOTPRES exception
 */
public class UbfBNOTPRESException extends UbfException {
        
    public UbfBNOTPRESException(String msg) {
        super(AtmiConst.BNOTPRES, msg);
    }
    
    public UbfBNOTPRESException(String msg, Throwable throwable) {
        super(AtmiConst.BNOTPRES, msg, throwable);
    }
}


