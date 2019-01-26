package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * NEMANDATORY exception
 */
public class NstdNEMANDATORYException extends NstdException {
        
    public NstdNEMANDATORYException(String msg) {
        super(AtmiConst.NEMANDATORY, msg);
    }
    
    public NstdNEMANDATORYException(String msg, Throwable throwable) {
        super(AtmiConst.NEMANDATORY, msg, throwable);
    }
}


