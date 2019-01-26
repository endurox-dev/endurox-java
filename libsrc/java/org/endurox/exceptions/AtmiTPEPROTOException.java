package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * TPEPROTO exception
 */
public class AtmiTPEPROTOException extends AtmiException {
        
    public AtmiTPEPROTOException(String msg) {
        super(AtmiConst.TPEPROTO, msg);
    }
    
    public AtmiTPEPROTOException(String msg, Throwable throwable) {
        super(AtmiConst.TPEPROTO, msg, throwable);
    }
}


