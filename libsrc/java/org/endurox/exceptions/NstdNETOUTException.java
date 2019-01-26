package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * NETOUT exception
 */
public class NstdNETOUTException extends NstdException {
        
    public NstdNETOUTException(String msg) {
        super(AtmiConst.NETOUT, msg);
    }
    
    public NstdNETOUTException(String msg, Throwable throwable) {
        super(AtmiConst.NETOUT, msg, throwable);
    }
}


