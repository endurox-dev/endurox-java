package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * TPEOTYPE exception
 */
public class AtmiTPEOTYPEException extends AtmiException {
        
    public AtmiTPEOTYPEException(String msg) {
        super(AtmiConstants.TPEOTYPE, msg);
    }
    
    public AtmiTPEOTYPEException(String msg, Throwable throwable) {
        super(AtmiConstants.TPEOTYPE, msg, throwable);
    }
}


