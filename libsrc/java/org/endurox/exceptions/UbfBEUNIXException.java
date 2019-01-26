package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * BEUNIX exception
 */
public class UbfBEUNIXException extends UbfException {
        
    public UbfBEUNIXException(String msg) {
        super(AtmiConst.BEUNIX, msg);
    }
    
    public UbfBEUNIXException(String msg, Throwable throwable) {
        super(AtmiConst.BEUNIX, msg, throwable);
    }
}


