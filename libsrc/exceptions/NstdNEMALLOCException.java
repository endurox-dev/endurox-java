package org.endurox;

/**
 * NEMALLOC exception
 */
public class NstdNEMALLOCException extends NstdException {
        
    public NstdNEMALLOCException(String msg) {
        super(msg, AtmiConstants.NEMALLOC);
    }
    
    public NstdNEMALLOCException(String s, Throwable throwable) {
        super(s, AtmiConstants.NEMALLOC, throwable);
    }
}


