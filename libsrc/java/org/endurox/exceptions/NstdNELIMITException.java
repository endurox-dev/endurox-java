package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * NELIMIT exception
 */
public class NstdNELIMITException extends NstdException {
        
    public NstdNELIMITException(String msg) {
        super(AtmiConst.NELIMIT, msg);
    }
    
    public NstdNELIMITException(String msg, Throwable throwable) {
        super(AtmiConst.NELIMIT, msg, throwable);
    }
}


