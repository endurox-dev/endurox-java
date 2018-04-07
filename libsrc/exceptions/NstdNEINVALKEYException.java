package org.endurox;

/**
 * NEINVALKEY exception
 */
public class NstdNEINVALKEYException extends NstdException {
        
    public NstdNEINVALKEYException(String msg) {
        super(msg, AtmiConstants.NEINVALKEY);
    }
    
    public NstdNEINVALKEYException(String s, Throwable throwable) {
        super(s, AtmiConstants.NEINVALKEY, throwable);
    }
}


