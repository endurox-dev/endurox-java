package org.endurox;

/**
 * NELIMIT exception
 */
public class NstdNELIMITException extends NstdException {
        
    public NstdNELIMITException(String msg) {
        super(AtmiConstants.NELIMIT, msg);
    }
    
    public NstdNELIMITException(String msg, Throwable throwable) {
        super(AtmiConstants.NELIMIT, msg, throwable);
    }
}


