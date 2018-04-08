package org.endurox;

/**
 * BFTOPEN exception
 */
public class UbfBFTOPENException extends UbfException {
        
    public UbfBFTOPENException(String msg) {
        super(AtmiConstants.BFTOPEN, msg);
    }
    
    public UbfBFTOPENException(String msg, Throwable throwable) {
        super(AtmiConstants.BFTOPEN, msg, throwable);
    }
}


