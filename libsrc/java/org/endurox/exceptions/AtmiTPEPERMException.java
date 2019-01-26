package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * TPEPERM exception
 */
public class AtmiTPEPERMException extends AtmiException {
        
    public AtmiTPEPERMException(String msg) {
        super(AtmiConst.TPEPERM, msg);
    }
    
    public AtmiTPEPERMException(String msg, Throwable throwable) {
        super(AtmiConst.TPEPERM, msg, throwable);
    }
}


