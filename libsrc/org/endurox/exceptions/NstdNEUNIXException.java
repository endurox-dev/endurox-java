package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * NEUNIX exception
 */
public class NstdNEUNIXException extends NstdException {
        
    public NstdNEUNIXException(String msg) {
        super(AtmiConstants.NEUNIX, msg);
    }
    
    public NstdNEUNIXException(String msg, Throwable throwable) {
        super(AtmiConstants.NEUNIX, msg, throwable);
    }
}


