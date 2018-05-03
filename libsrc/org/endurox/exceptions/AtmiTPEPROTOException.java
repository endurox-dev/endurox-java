package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * TPEPROTO exception
 */
public class AtmiTPEPROTOException extends AtmiException {
        
    public AtmiTPEPROTOException(String msg) {
        super(AtmiConstants.TPEPROTO, msg);
    }
    
    public AtmiTPEPROTOException(String msg, Throwable throwable) {
        super(AtmiConstants.TPEPROTO, msg, throwable);
    }
}


