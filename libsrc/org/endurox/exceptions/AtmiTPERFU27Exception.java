package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * TPERFU27 exception
 */
public class AtmiTPERFU27Exception extends AtmiException {
        
    public AtmiTPERFU27Exception(String msg) {
        super(AtmiConstants.TPERFU27, msg);
    }
    
    public AtmiTPERFU27Exception(String msg, Throwable throwable) {
        super(AtmiConstants.TPERFU27, msg, throwable);
    }
}


