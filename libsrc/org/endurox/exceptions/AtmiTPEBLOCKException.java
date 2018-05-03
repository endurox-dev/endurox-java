package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * TPEBLOCK exception
 */
public class AtmiTPEBLOCKException extends AtmiException {
        
    public AtmiTPEBLOCKException(String msg) {
        super(AtmiConstants.TPEBLOCK, msg);
    }
    
    public AtmiTPEBLOCKException(String msg, Throwable throwable) {
        super(AtmiConstants.TPEBLOCK, msg, throwable);
    }
}


