package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * BEINVAL exception
 */
public class UbfBEINVALException extends UbfException {
        
    public UbfBEINVALException(String msg) {
        super(AtmiConst.BEINVAL, msg);
    }
    
    public UbfBEINVALException(String msg, Throwable throwable) {
        super(AtmiConst.BEINVAL, msg, throwable);
    }
}


