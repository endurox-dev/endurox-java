package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * BFTOPEN exception
 */
public class UbfBFTOPENException extends UbfException {
        
    public UbfBFTOPENException(String msg) {
        super(AtmiConst.BFTOPEN, msg);
    }
    
    public UbfBFTOPENException(String msg, Throwable throwable) {
        super(AtmiConst.BFTOPEN, msg, throwable);
    }
}


