package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * TPGOTSIG exception
 */
public class AtmiTPGOTSIGException extends AtmiException {
        
    public AtmiTPGOTSIGException(String msg) {
        super(AtmiConstants.TPGOTSIG, msg);
    }
    
    public AtmiTPGOTSIGException(String msg, Throwable throwable) {
        super(AtmiConstants.TPGOTSIG, msg, throwable);
    }
}


