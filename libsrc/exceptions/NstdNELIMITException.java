package org.endurox;

/**
 * NELIMIT exception
 */
public class NstdNELIMITException extends NstdException {
        
    public NstdNELIMITException(String msg) {
        super(msg, AtmiConstants.NELIMIT);
    }
    
    public NstdNELIMITException(String s, Throwable throwable) {
        super(s, AtmiConstants.NELIMIT, throwable);
    }
}


