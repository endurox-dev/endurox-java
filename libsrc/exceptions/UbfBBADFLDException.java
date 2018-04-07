package org.endurox;

/**
 * BBADFLD exception
 */
public class UbfBBADFLDException extends UbfException {
        
    public UbfBBADFLDException(String msg) {
        super(msg, AtmiConstants.BBADFLD);
    }
    
    public UbfBBADFLDException(String s, Throwable throwable) {
        super(s, AtmiConstants.BBADFLD, throwable);
    }
}


