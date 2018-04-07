package org.endurox;

/**
 * BBADTBL exception
 */
public class UbfBBADTBLException extends UbfException {
        
    public UbfBBADTBLException(String msg) {
        super(msg, AtmiConstants.BBADTBL);
    }
    
    public UbfBBADTBLException(String s, Throwable throwable) {
        super(s, AtmiConstants.BBADTBL, throwable);
    }
}


