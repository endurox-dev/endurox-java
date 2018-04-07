package org.endurox;

/**
 * BNOSPACE exception
 */
public class UbfBNOSPACEException extends UbfException {
        
    public UbfBNOSPACEException(String msg) {
        super(msg, AtmiConstants.BNOSPACE);
    }
    
    public UbfBNOSPACEException(String s, Throwable throwable) {
        super(s, AtmiConstants.BNOSPACE, throwable);
    }
}


