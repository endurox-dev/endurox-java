package org.endurox;

/**
 * TPEBLOCK exception
 */
public class AtmiTPEBLOCKException extends AtmiException {
        
    public AtmiTPEBLOCKException(String msg) {
        super(msg, AtmiConstants.TPEBLOCK);
    }
    
    public AtmiTPEBLOCKException(String s, Throwable throwable) {
        super(s, AtmiConstants.TPEBLOCK, throwable);
    }
}


