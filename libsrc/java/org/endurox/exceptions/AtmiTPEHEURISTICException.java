package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * TPEHEURISTIC exception
 */
public class AtmiTPEHEURISTICException extends AtmiException {
        
    public AtmiTPEHEURISTICException(String msg) {
        super(AtmiConst.TPEHEURISTIC, msg);
    }
    
    public AtmiTPEHEURISTICException(String msg, Throwable throwable) {
        super(AtmiConst.TPEHEURISTIC, msg, throwable);
    }
}


