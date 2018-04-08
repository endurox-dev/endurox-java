package org.endurox;

/**
 * BEUNIX exception
 */
public class UbfBEUNIXException extends UbfException {
        
    public UbfBEUNIXException(String msg) {
        super(AtmiConstants.BEUNIX, msg);
    }
    
    public UbfBEUNIXException(String msg, Throwable throwable) {
        super(AtmiConstants.BEUNIX, msg, throwable);
    }
}


