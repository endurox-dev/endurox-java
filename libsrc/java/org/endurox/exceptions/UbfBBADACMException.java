package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * BBADACM exception
 */
public class UbfBBADACMException extends UbfException {
        
    public UbfBBADACMException(String msg) {
        super(AtmiConstants.BBADACM, msg);
    }
    
    public UbfBBADACMException(String msg, Throwable throwable) {
        super(AtmiConstants.BBADACM, msg, throwable);
    }
}

