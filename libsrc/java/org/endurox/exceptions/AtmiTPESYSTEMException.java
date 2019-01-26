package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * TPESYSTEM exception
 */
public class AtmiTPESYSTEMException extends AtmiException {
        
    public AtmiTPESYSTEMException(String msg) {
        super(AtmiConst.TPESYSTEM, msg);
    }
    
    public AtmiTPESYSTEMException(String msg, Throwable throwable) {
        super(AtmiConst.TPESYSTEM, msg, throwable);
    }
}


