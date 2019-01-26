package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * BTYPERR exception
 */
public class UbfBTYPERRException extends UbfException {
        
    public UbfBTYPERRException(String msg) {
        super(AtmiConst.BTYPERR, msg);
    }
    
    public UbfBTYPERRException(String msg, Throwable throwable) {
        super(AtmiConst.BTYPERR, msg, throwable);
    }
}


