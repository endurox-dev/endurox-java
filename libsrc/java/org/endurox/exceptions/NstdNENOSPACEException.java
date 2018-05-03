package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * NENOSPACE exception
 */
public class NstdNENOSPACEException extends NstdException {
        
    public NstdNENOSPACEException(String msg) {
        super(AtmiConstants.NENOSPACE, msg);
    }
    
    public NstdNENOSPACEException(String msg, Throwable throwable) {
        super(AtmiConstants.NENOSPACE, msg, throwable);
    }
}

