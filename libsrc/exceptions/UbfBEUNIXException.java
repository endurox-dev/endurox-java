package org.endurox;

/**
 * BEUNIX exception
 */
public class UbfBEUNIXException extends UbfException {
        
    public UbfBEUNIXException(String msg) {
        super(msg, AtmiConstants.BEUNIX);
    }
    
    public UbfBEUNIXException(String s, Throwable throwable) {
        super(s, AtmiConstants.BEUNIX, throwable);
    }
}


