package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * NENOSPACE exception
 */
public class NstdNENOSPACEException extends NstdException {
        
    public NstdNENOSPACEException(String msg) {
        super(AtmiConst.NENOSPACE, msg);
    }
    
    public NstdNENOSPACEException(String msg, Throwable throwable) {
        super(AtmiConst.NENOSPACE, msg, throwable);
    }
}


