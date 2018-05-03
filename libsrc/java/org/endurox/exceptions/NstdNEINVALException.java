package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * NEINVAL exception
 */
public class NstdNEINVALException extends NstdException {
        
    public NstdNEINVALException(String msg) {
        super(AtmiConstants.NEINVAL, msg);
    }
    
    public NstdNEINVALException(String msg, Throwable throwable) {
        super(AtmiConstants.NEINVAL, msg, throwable);
    }
}


