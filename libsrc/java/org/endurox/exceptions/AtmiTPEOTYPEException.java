package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * TPEOTYPE exception
 */
public class AtmiTPEOTYPEException extends AtmiException {
        
    public AtmiTPEOTYPEException(String msg) {
        super(AtmiConst.TPEOTYPE, msg);
    }
    
    public AtmiTPEOTYPEException(String msg, Throwable throwable) {
        super(AtmiConst.TPEOTYPE, msg, throwable);
    }
}


