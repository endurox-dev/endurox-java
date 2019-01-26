package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * NEINVALKEY exception
 */
public class NstdNEINVALKEYException extends NstdException {
        
    public NstdNEINVALKEYException(String msg) {
        super(AtmiConst.NEINVALKEY, msg);
    }
    
    public NstdNEINVALKEYException(String msg, Throwable throwable) {
        super(AtmiConst.NEINVALKEY, msg, throwable);
    }
}


