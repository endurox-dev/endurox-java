package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * TPEPERM exception
 */
public class AtmiTPEPERMException extends AtmiException {
        
    public AtmiTPEPERMException(String msg) {
        super(AtmiConstants.TPEPERM, msg);
    }
    
    public AtmiTPEPERMException(String msg, Throwable throwable) {
        super(AtmiConstants.TPEPERM, msg, throwable);
    }
}


