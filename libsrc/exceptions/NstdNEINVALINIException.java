package org.endurox;

/**
 * NEINVALINI exception
 */
public class NstdNEINVALINIException extends NstdException {
        
    public NstdNEINVALINIException(String msg) {
        super(msg, AtmiConstants.NEINVALINI);
    }
    
    public NstdNEINVALINIException(String s, Throwable throwable) {
        super(s, AtmiConstants.NEINVALINI, throwable);
    }
}


