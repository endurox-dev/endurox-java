package org.endurox;

/**
 * NEFORMAT exception
 */
public class NstdNEFORMATException extends NstdException {
        
    public NstdNEFORMATException(String msg) {
        super(AtmiConstants.NEFORMAT, msg);
    }
    
    public NstdNEFORMATException(String msg, Throwable throwable) {
        super(AtmiConstants.NEFORMAT, msg, throwable);
    }
}


