package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * BNOSPACE exception
 */
public class UbfBNOSPACEException extends UbfException {
        
    public UbfBNOSPACEException(String msg) {
        super(AtmiConst.BNOSPACE, msg);
    }
    
    public UbfBNOSPACEException(String msg, Throwable throwable) {
        super(AtmiConst.BNOSPACE, msg, throwable);
    }
}


