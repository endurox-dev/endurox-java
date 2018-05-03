package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * TPETRAN exception
 */
public class AtmiTPETRANException extends AtmiException {
        
    public AtmiTPETRANException(String msg) {
        super(AtmiConstants.TPETRAN, msg);
    }
    
    public AtmiTPETRANException(String msg, Throwable throwable) {
        super(AtmiConstants.TPETRAN, msg, throwable);
    }
}


