package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * TPEITYPE exception
 */
public class AtmiTPEITYPEException extends AtmiException {
        
    public AtmiTPEITYPEException(String msg) {
        super(AtmiConst.TPEITYPE, msg);
    }
    
    public AtmiTPEITYPEException(String msg, Throwable throwable) {
        super(AtmiConst.TPEITYPE, msg, throwable);
    }
}


