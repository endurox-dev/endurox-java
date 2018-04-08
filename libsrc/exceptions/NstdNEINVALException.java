package org.endurox;

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


