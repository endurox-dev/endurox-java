package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * NEUNIX exception
 */
public class NstdNEUNIXException extends NstdException {
        
    public NstdNEUNIXException(String msg) {
        super(AtmiConst.NEUNIX, msg);
    }
    
    public NstdNEUNIXException(String msg, Throwable throwable) {
        super(AtmiConst.NEUNIX, msg, throwable);
    }
}


