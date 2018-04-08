package org.endurox;

/**
 * NEINVALINI exception
 */
public class NstdNEINVALINIException extends NstdException {
        
    public NstdNEINVALINIException(String msg) {
        super(AtmiConstants.NEINVALINI, msg);
    }
    
    public NstdNEINVALINIException(String msg, Throwable throwable) {
        super(AtmiConstants.NEINVALINI, msg, throwable);
    }
}


