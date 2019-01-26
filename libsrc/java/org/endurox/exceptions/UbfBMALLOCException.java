package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * BMALLOC exception
 */
public class UbfBMALLOCException extends UbfException {
        
    public UbfBMALLOCException(String msg) {
        super(AtmiConst.BMALLOC, msg);
    }
    
    public UbfBMALLOCException(String msg, Throwable throwable) {
        super(AtmiConst.BMALLOC, msg, throwable);
    }
}


