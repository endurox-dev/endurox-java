package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * NEMALLOC exception
 */
public class NstdNEMALLOCException extends NstdException {
        
    public NstdNEMALLOCException(String msg) {
        super(AtmiConstants.NEMALLOC, msg);
    }
    
    public NstdNEMALLOCException(String msg, Throwable throwable) {
        super(AtmiConstants.NEMALLOC, msg, throwable);
    }
}


