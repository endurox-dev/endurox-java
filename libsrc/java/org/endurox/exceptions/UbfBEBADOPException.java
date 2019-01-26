package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * BEBADOP exception
 */
public class UbfBEBADOPException extends UbfException {
        
    public UbfBEBADOPException(String msg) {
        super(AtmiConst.BEBADOP, msg);
    }
    
    public UbfBEBADOPException(String msg, Throwable throwable) {
        super(AtmiConst.BEBADOP, msg, throwable);
    }
}


