package org.endurox;

/**
 * TPEHEURISTIC exception
 */
public class AtmiTPEHEURISTICException extends AtmiException {
        
    public AtmiTPEHEURISTICException(String msg) {
        super(msg, AtmiConstants.TPEHEURISTIC);
    }
    
    public AtmiTPEHEURISTICException(String s, Throwable throwable) {
        super(s, AtmiConstants.TPEHEURISTIC, throwable);
    }
}


