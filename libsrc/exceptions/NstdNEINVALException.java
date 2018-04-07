package org.endurox;

/**
 * NEINVAL exception
 */
public class NstdNEINVALException extends NstdException {
        
    public NstdNEINVALException(String msg) {
        super(msg, AtmiConstants.NEINVAL);
    }
    
    public NstdNEINVALException(String s, Throwable throwable) {
        super(s, AtmiConstants.NEINVAL, throwable);
    }
}


