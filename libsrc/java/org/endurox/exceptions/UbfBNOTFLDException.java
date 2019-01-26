package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * BNOTFLD exception
 */
public class UbfBNOTFLDException extends UbfException {
        
    public UbfBNOTFLDException(String msg) {
        super(AtmiConst.BNOTFLD, msg);
    }
    
    public UbfBNOTFLDException(String msg, Throwable throwable) {
        super(AtmiConst.BNOTFLD, msg, throwable);
    }
}


