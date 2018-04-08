package org.endurox;

/**
 * BMALLOC exception
 */
public class UbfBMALLOCException extends UbfException {
        
    public UbfBMALLOCException(String msg) {
        super(AtmiConstants.BMALLOC, msg);
    }
    
    public UbfBMALLOCException(String msg, Throwable throwable) {
        super(AtmiConstants.BMALLOC, msg, throwable);
    }
}


