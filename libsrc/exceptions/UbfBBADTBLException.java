package org.endurox;

/**
 * BBADTBL exception
 */
public class UbfBBADTBLException extends UbfException {
        
    public UbfBBADTBLException(String msg) {
        super(AtmiConstants.BBADTBL, msg);
    }
    
    public UbfBBADTBLException(String msg, Throwable throwable) {
        super(AtmiConstants.BBADTBL, msg, throwable);
    }
}


