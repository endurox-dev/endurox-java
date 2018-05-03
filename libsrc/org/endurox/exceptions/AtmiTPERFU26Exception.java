package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * TPERFU26 exception
 */
public class AtmiTPERFU26Exception extends AtmiException {
        
    public AtmiTPERFU26Exception(String msg) {
        super(AtmiConstants.TPERFU26, msg);
    }
    
    public AtmiTPERFU26Exception(String msg, Throwable throwable) {
        super(AtmiConstants.TPERFU26, msg, throwable);
    }
}


