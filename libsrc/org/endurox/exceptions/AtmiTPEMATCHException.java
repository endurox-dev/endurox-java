package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * TPEMATCH exception
 */
public class AtmiTPEMATCHException extends AtmiException {
        
    public AtmiTPEMATCHException(String msg) {
        super(AtmiConstants.TPEMATCH, msg);
    }
    
    public AtmiTPEMATCHException(String msg, Throwable throwable) {
        super(AtmiConstants.TPEMATCH, msg, throwable);
    }
}


