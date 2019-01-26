package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * NEMALLOC exception
 */
public class NstdNEMALLOCException extends NstdException {
        
    public NstdNEMALLOCException(String msg) {
        super(AtmiConst.NEMALLOC, msg);
    }
    
    public NstdNEMALLOCException(String msg, Throwable throwable) {
        super(AtmiConst.NEMALLOC, msg, throwable);
    }
}


