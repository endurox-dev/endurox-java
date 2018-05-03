package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * NESYSTEM exception
 */
public class NstdNESYSTEMException extends NstdException {
        
    public NstdNESYSTEMException(String msg) {
        super(AtmiConstants.NESYSTEM, msg);
    }
    
    public NstdNESYSTEMException(String msg, Throwable throwable) {
        super(AtmiConstants.NESYSTEM, msg, throwable);
    }
}

