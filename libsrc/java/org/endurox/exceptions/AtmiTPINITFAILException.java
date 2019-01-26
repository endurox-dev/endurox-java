package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * TPINITFAIL exception
 */
public class AtmiTPINITFAILException extends AtmiException {
        
    public AtmiTPINITFAILException(String msg) {
        super(AtmiConst.TPINITFAIL, msg);
    }
    
    public AtmiTPINITFAILException(String msg, Throwable throwable) {
        super(AtmiConst.TPINITFAIL, msg, throwable);
    }
}


