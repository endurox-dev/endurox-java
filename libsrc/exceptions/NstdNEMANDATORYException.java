package org.endurox;

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


