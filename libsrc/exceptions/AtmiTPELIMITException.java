package org.endurox;

/**
 * TPELIMIT exception
 */
public class AtmiTPELIMITException extends AtmiException {
        
    public AtmiTPELIMITException(String msg) {
        super(msg, AtmiConstants.TPELIMIT);
    }
    
    public AtmiTPELIMITException(String s, Throwable throwable) {
        super(s, AtmiConstants.TPELIMIT, throwable);
    }
}


