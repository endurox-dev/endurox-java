package org.endurox;

/**
 * NEUNIX exception
 */
public class NstdNEUNIXException extends NstdException {
        
    public NstdNEUNIXException(String msg) {
        super(msg, AtmiConstants.NEUNIX);
    }
    
    public NstdNEUNIXException(String s, Throwable throwable) {
        super(s, AtmiConstants.NEUNIX, throwable);
    }
}


