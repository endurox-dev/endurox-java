package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * TPETIME exception
 */
public class AtmiTPETIMEException extends AtmiException {
        
    public AtmiTPETIMEException(String msg) {
        super(AtmiConst.TPETIME, msg);
    }
    
    public AtmiTPETIMEException(String msg, Throwable throwable) {
        super(AtmiConst.TPETIME, msg, throwable);
    }
}


