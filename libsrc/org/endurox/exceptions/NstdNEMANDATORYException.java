package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * NEMANDATORY exception
 */
public class NstdNEMANDATORYException extends NstdException {
        
    public NstdNEMANDATORYException(String msg) {
        super(AtmiConstants.NEMANDATORY, msg);
    }
    
    public NstdNEMANDATORYException(String msg, Throwable throwable) {
        super(AtmiConstants.NEMANDATORY, msg, throwable);
    }
}


