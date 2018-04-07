package org.endurox;

/**
 * BMALLOC exception
 */
public class UbfBMALLOCException extends UbfException {
        
    public UbfBMALLOCException(String msg) {
        super(msg, AtmiConstants.BMALLOC);
    }
    
    public UbfBMALLOCException(String s, Throwable throwable) {
        super(s, AtmiConstants.BMALLOC, throwable);
    }
}


