package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * BBADFLD exception
 */
public class UbfBBADFLDException extends UbfException {
        
    public UbfBBADFLDException(String msg) {
        super(AtmiConst.BBADFLD, msg);
    }
    
    public UbfBBADFLDException(String msg, Throwable throwable) {
        super(AtmiConst.BBADFLD, msg, throwable);
    }
}


