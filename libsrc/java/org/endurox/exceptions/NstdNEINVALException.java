package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * NEINVAL exception
 */
public class NstdNEINVALException extends NstdException {
        
    public NstdNEINVALException(String msg) {
        super(AtmiConst.NEINVAL, msg);
    }
    
    public NstdNEINVALException(String msg, Throwable throwable) {
        super(AtmiConst.NEINVAL, msg, throwable);
    }
}


