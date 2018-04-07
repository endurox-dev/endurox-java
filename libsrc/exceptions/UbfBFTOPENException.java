package org.endurox;

/**
 * BFTOPEN exception
 */
public class UbfBFTOPENException extends UbfException {
        
    public UbfBFTOPENException(String msg) {
        super(msg, AtmiConstants.BFTOPEN);
    }
    
    public UbfBFTOPENException(String s, Throwable throwable) {
        super(s, AtmiConstants.BFTOPEN, throwable);
    }
}


