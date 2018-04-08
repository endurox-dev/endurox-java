package org.endurox;

/**
 * BEBADOP exception
 */
public class UbfBEBADOPException extends UbfException {
        
    public UbfBEBADOPException(String msg) {
        super(AtmiConstants.BEBADOP, msg);
    }
    
    public UbfBEBADOPException(String msg, Throwable throwable) {
        super(AtmiConstants.BEBADOP, msg, throwable);
    }
}


