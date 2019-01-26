package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * TPEBADDESC exception
 */
public class AtmiTPEBADDESCException extends AtmiException {
        
    public AtmiTPEBADDESCException(String msg) {
        super(AtmiConst.TPEBADDESC, msg);
    }
    
    public AtmiTPEBADDESCException(String msg, Throwable throwable) {
        super(AtmiConst.TPEBADDESC, msg, throwable);
    }
}


