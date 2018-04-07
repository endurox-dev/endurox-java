package org.endurox;

/**
 * NEFORMAT exception
 */
public class NstdNEFORMATException extends NstdException {
        
    public NstdNEFORMATException(String msg) {
        super(msg, AtmiConstants.NEFORMAT);
    }
    
    public NstdNEFORMATException(String s, Throwable throwable) {
        super(s, AtmiConstants.NEFORMAT, throwable);
    }
}


