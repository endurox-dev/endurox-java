package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * NEINVALINI exception
 */
public class NstdNEINVALINIException extends NstdException {
        
    public NstdNEINVALINIException(String msg) {
        super(AtmiConst.NEINVALINI, msg);
    }
    
    public NstdNEINVALINIException(String msg, Throwable throwable) {
        super(AtmiConst.NEINVALINI, msg, throwable);
    }
}


