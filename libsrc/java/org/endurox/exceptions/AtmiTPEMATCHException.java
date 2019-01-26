package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * TPEMATCH exception
 */
public class AtmiTPEMATCHException extends AtmiException {
        
    public AtmiTPEMATCHException(String msg) {
        super(AtmiConst.TPEMATCH, msg);
    }
    
    public AtmiTPEMATCHException(String msg, Throwable throwable) {
        super(AtmiConst.TPEMATCH, msg, throwable);
    }
}


