package org.endurox;

/**
 * NETOUT exception
 */
public class NstdNETOUTException extends NstdException {
        
    public NstdNETOUTException(String msg) {
        super(msg, AtmiConstants.NETOUT);
    }
    
    public NstdNETOUTException(String s, Throwable throwable) {
        super(s, AtmiConstants.NETOUT, throwable);
    }
}


