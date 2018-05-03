package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * TPEHEURISTIC exception
 */
public class AtmiTPEHEURISTICException extends AtmiException {
        
    public AtmiTPEHEURISTICException(String msg) {
        super(AtmiConstants.TPEHEURISTIC, msg);
    }
    
    public AtmiTPEHEURISTICException(String msg, Throwable throwable) {
        super(AtmiConstants.TPEHEURISTIC, msg, throwable);
    }
}


