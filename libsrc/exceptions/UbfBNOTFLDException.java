package org.endurox;

/**
 * BNOTFLD exception
 */
public class UbfBNOTFLDException extends UbfException {
        
    public UbfBNOTFLDException(String msg) {
        super(msg, AtmiConstants.BNOTFLD);
    }
    
    public UbfBNOTFLDException(String s, Throwable throwable) {
        super(s, AtmiConstants.BNOTFLD, throwable);
    }
}


