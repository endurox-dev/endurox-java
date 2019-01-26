package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * TPERFU29 exception
 */
public class AtmiTPERFU29Exception extends AtmiException {
        
    public AtmiTPERFU29Exception(String msg) {
        super(AtmiConst.TPERFU29, msg);
    }
    
    public AtmiTPERFU29Exception(String msg, Throwable throwable) {
        super(AtmiConst.TPERFU29, msg, throwable);
    }
}


