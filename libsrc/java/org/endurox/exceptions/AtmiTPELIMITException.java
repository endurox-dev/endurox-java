package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * TPELIMIT exception
 */
public class AtmiTPELIMITException extends AtmiException {
        
    public AtmiTPELIMITException(String msg) {
        super(AtmiConst.TPELIMIT, msg);
    }
    
    public AtmiTPELIMITException(String msg, Throwable throwable) {
        super(AtmiConst.TPELIMIT, msg, throwable);
    }
}


