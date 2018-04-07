package org.endurox;

/**
 * BFTSYNTAX exception
 */
public class UbfBFTSYNTAXException extends UbfException {
        
    public UbfBFTSYNTAXException(String msg) {
        super(msg, AtmiConstants.BFTSYNTAX);
    }
    
    public UbfBFTSYNTAXException(String s, Throwable throwable) {
        super(s, AtmiConstants.BFTSYNTAX, throwable);
    }
}


