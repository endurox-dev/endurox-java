package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * TPERFU27 exception
 */
public class AtmiTPERFU27Exception extends AtmiException {
        
    public AtmiTPERFU27Exception(String msg) {
        super(AtmiConst.TPERFU27, msg);
    }
    
    public AtmiTPERFU27Exception(String msg, Throwable throwable) {
        super(AtmiConst.TPERFU27, msg, throwable);
    }
}


