package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * TPEBLOCK exception
 */
public class AtmiTPEBLOCKException extends AtmiException {
        
    public AtmiTPEBLOCKException(String msg) {
        super(AtmiConst.TPEBLOCK, msg);
    }
    
    public AtmiTPEBLOCKException(String msg, Throwable throwable) {
        super(AtmiConst.TPEBLOCK, msg, throwable);
    }
}


