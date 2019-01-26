package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * TPETRAN exception
 */
public class AtmiTPETRANException extends AtmiException {
        
    public AtmiTPETRANException(String msg) {
        super(AtmiConst.TPETRAN, msg);
    }
    
    public AtmiTPETRANException(String msg, Throwable throwable) {
        super(AtmiConst.TPETRAN, msg, throwable);
    }
}


