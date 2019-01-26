package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * TPERFU26 exception
 */
public class AtmiTPERFU26Exception extends AtmiException {
        
    public AtmiTPERFU26Exception(String msg) {
        super(AtmiConst.TPERFU26, msg);
    }
    
    public AtmiTPERFU26Exception(String msg, Throwable throwable) {
        super(AtmiConst.TPERFU26, msg, throwable);
    }
}


