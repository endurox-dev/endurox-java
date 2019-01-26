package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * BBADACM exception
 */
public class UbfBBADACMException extends UbfException {
        
    public UbfBBADACMException(String msg) {
        super(AtmiConst.BBADACM, msg);
    }
    
    public UbfBBADACMException(String msg, Throwable throwable) {
        super(AtmiConst.BBADACM, msg, throwable);
    }
}


