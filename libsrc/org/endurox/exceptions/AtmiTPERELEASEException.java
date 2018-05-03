package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * TPERELEASE exception
 */
public class AtmiTPERELEASEException extends AtmiException {
        
    public AtmiTPERELEASEException(String msg) {
        super(AtmiConstants.TPERELEASE, msg);
    }
    
    public AtmiTPERELEASEException(String msg, Throwable throwable) {
        super(AtmiConstants.TPERELEASE, msg, throwable);
    }
}


