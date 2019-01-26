package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * BBADTBL exception
 */
public class UbfBBADTBLException extends UbfException {
        
    public UbfBBADTBLException(String msg) {
        super(AtmiConst.BBADTBL, msg);
    }
    
    public UbfBBADTBLException(String msg, Throwable throwable) {
        super(AtmiConst.BBADTBL, msg, throwable);
    }
}


