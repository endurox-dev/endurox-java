package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * NESYSTEM exception
 */
public class NstdNESYSTEMException extends NstdException {
        
    public NstdNESYSTEMException(String msg) {
        super(AtmiConst.NESYSTEM, msg);
    }
    
    public NstdNESYSTEMException(String msg, Throwable throwable) {
        super(AtmiConst.NESYSTEM, msg, throwable);
    }
}


