package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * BBADTBL exception
 */
public class UbfBBADTBLException extends UbfException {
        
    public UbfBBADTBLException(String msg) {
        super(AtmiConstants.BBADTBL, msg);
    }
    
    public UbfBBADTBLException(String msg, Throwable throwable) {
        super(AtmiConstants.BBADTBL, msg, throwable);
    }
}


