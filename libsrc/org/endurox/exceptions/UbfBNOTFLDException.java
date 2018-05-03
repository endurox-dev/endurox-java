package org.endurox.exceptions;
import org.endurox.AtmiConstants;

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


