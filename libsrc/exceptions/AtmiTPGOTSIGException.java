package org.endurox;

/**
 * TPGOTSIG exception
 */
public class AtmiTPGOTSIGException extends AtmiException {
        
    public AtmiTPGOTSIGException(String msg) {
        super(msg, AtmiConstants.TPGOTSIG);
    }
    
    public AtmiTPGOTSIGException(String s, Throwable throwable) {
        super(s, AtmiConstants.TPGOTSIG, throwable);
    }
}


