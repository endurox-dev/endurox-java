package org.endurox;

/**
 * NEMANDATORY exception
 */
public class NstdNEMANDATORYException extends NstdException {
        
    public NstdNEMANDATORYException(String msg) {
        super(msg, AtmiConstants.NEMANDATORY);
    }
    
    public NstdNEMANDATORYException(String s, Throwable throwable) {
        super(s, AtmiConstants.NEMANDATORY, throwable);
    }
}


