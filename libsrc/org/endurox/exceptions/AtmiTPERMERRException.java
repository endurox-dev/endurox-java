package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * TPERMERR exception
 */
public class AtmiTPERMERRException extends AtmiException {
        
    public AtmiTPERMERRException(String msg) {
        super(AtmiConstants.TPERMERR, msg);
    }
    
    public AtmiTPERMERRException(String msg, Throwable throwable) {
        super(AtmiConstants.TPERMERR, msg, throwable);
    }
}


