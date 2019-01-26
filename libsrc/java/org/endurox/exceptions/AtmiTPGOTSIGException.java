package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * TPGOTSIG exception
 */
public class AtmiTPGOTSIGException extends AtmiException {
        
    public AtmiTPGOTSIGException(String msg) {
        super(AtmiConst.TPGOTSIG, msg);
    }
    
    public AtmiTPGOTSIGException(String msg, Throwable throwable) {
        super(AtmiConst.TPGOTSIG, msg, throwable);
    }
}


