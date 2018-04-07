package org.endurox;

/**
 * NENOSPACE exception
 */
public class NstdNENOSPACEException extends NstdException {
        
    public NstdNENOSPACEException(String msg) {
        super(msg, AtmiConstants.NENOSPACE);
    }
    
    public NstdNENOSPACEException(String s, Throwable throwable) {
        super(s, AtmiConstants.NENOSPACE, throwable);
    }
}


