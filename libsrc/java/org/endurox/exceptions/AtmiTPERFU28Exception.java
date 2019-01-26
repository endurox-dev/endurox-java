package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * TPERFU28 exception
 */
public class AtmiTPERFU28Exception extends AtmiException {
        
    public AtmiTPERFU28Exception(String msg) {
        super(AtmiConst.TPERFU28, msg);
    }
    
    public AtmiTPERFU28Exception(String msg, Throwable throwable) {
        super(AtmiConst.TPERFU28, msg, throwable);
    }
}


