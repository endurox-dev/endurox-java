package org.endurox;

/**
 * BNOTFLD exception
 */
public class UbfBNOTFLDException extends UbfException {
        
    public UbfBNOTFLDException(String msg) {
        super(AtmiConstants.BNOTFLD, msg);
    }
    
    public UbfBNOTFLDException(String msg, Throwable throwable) {
        super(AtmiConstants.BNOTFLD, msg, throwable);
    }
}


