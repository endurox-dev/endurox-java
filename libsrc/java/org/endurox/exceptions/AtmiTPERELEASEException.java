package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * TPERELEASE exception
 */
public class AtmiTPERELEASEException extends AtmiException {
        
    public AtmiTPERELEASEException(String msg) {
        super(AtmiConst.TPERELEASE, msg);
    }
    
    public AtmiTPERELEASEException(String msg, Throwable throwable) {
        super(AtmiConst.TPERELEASE, msg, throwable);
    }
}


