package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * TPESYSTEM exception
 */
public class AtmiTPESYSTEMException extends AtmiException {
        
    public AtmiTPESYSTEMException(String msg) {
        super(AtmiConstants.TPESYSTEM, msg);
    }
    
    public AtmiTPESYSTEMException(String msg, Throwable throwable) {
        super(AtmiConstants.TPESYSTEM, msg, throwable);
    }
}


