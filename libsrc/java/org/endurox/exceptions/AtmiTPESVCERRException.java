package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * TPESVCERR exception
 */
public class AtmiTPESVCERRException extends AtmiException {
        
    public AtmiTPESVCERRException(String msg) {
        super(AtmiConst.TPESVCERR, msg);
    }
    
    public AtmiTPESVCERRException(String msg, Throwable throwable) {
        super(AtmiConst.TPESVCERR, msg, throwable);
    }
}


