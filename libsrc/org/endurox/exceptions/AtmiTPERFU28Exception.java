package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * TPERFU28 exception
 */
public class AtmiTPERFU28Exception extends AtmiException {
        
    public AtmiTPERFU28Exception(String msg) {
        super(AtmiConstants.TPERFU28, msg);
    }
    
    public AtmiTPERFU28Exception(String msg, Throwable throwable) {
        super(AtmiConstants.TPERFU28, msg, throwable);
    }
}


