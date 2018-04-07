package org.endurox;

/**
 * BEBADOP exception
 */
public class UbfBEBADOPException extends UbfException {
        
    public UbfBEBADOPException(String msg) {
        super(msg, AtmiConstants.BEBADOP);
    }
    
    public UbfBEBADOPException(String s, Throwable throwable) {
        super(s, AtmiConstants.BEBADOP, throwable);
    }
}


