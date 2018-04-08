package org.endurox;

/**
 * BNOSPACE exception
 */
public class UbfBNOSPACEException extends UbfException {
        
    public UbfBNOSPACEException(String msg) {
        super(AtmiConstants.BNOSPACE, msg);
    }
    
    public UbfBNOSPACEException(String msg, Throwable throwable) {
        super(AtmiConstants.BNOSPACE, msg, throwable);
    }
}


