package org.endurox;

/**
 * NEPLUGIN exception
 */
public class NstdNEPLUGINException extends NstdException {
        
    public NstdNEPLUGINException(String msg) {
        super(msg, AtmiConstants.NEPLUGIN);
    }
    
    public NstdNEPLUGINException(String s, Throwable throwable) {
        super(s, AtmiConstants.NEPLUGIN, throwable);
    }
}


