package org.endurox;

/**
 * NESYSTEM exception
 */
public class NstdNESYSTEMException extends NstdException {
        
    public NstdNESYSTEMException(String msg) {
        super(msg, AtmiConstants.NESYSTEM);
    }
    
    public NstdNESYSTEMException(String s, Throwable throwable) {
        super(s, AtmiConstants.NESYSTEM, throwable);
    }
}


