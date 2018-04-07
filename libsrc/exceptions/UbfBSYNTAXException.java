package org.endurox;

/**
 * BSYNTAX exception
 */
public class UbfBSYNTAXException extends UbfException {
        
    public UbfBSYNTAXException(String msg) {
        super(msg, AtmiConstants.BSYNTAX);
    }
    
    public UbfBSYNTAXException(String s, Throwable throwable) {
        super(s, AtmiConstants.BSYNTAX, throwable);
    }
}


