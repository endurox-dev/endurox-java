package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * TPETIME exception
 */
public class AtmiTPETIMEException extends AtmiException {
        
    public AtmiTPETIMEException(String msg) {
        super(AtmiConstants.TPETIME, msg);
    }
    
    public AtmiTPETIMEException(String msg, Throwable throwable) {
        super(AtmiConstants.TPETIME, msg, throwable);
    }
}


