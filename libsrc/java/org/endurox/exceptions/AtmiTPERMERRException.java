package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * TPERMERR exception
 */
public class AtmiTPERMERRException extends AtmiException {
        
    public AtmiTPERMERRException(String msg) {
        super(AtmiConst.TPERMERR, msg);
    }
    
    public AtmiTPERMERRException(String msg, Throwable throwable) {
        super(AtmiConst.TPERMERR, msg, throwable);
    }
}


