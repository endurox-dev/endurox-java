package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * NEFORMAT exception
 */
public class NstdNEFORMATException extends NstdException {
        
    public NstdNEFORMATException(String msg) {
        super(AtmiConst.NEFORMAT, msg);
    }
    
    public NstdNEFORMATException(String msg, Throwable throwable) {
        super(AtmiConst.NEFORMAT, msg, throwable);
    }
}


