package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * BTYPERR exception
 */
public class UbfBTYPERRException extends UbfException {
        
    public UbfBTYPERRException(String msg) {
        super(AtmiConstants.BTYPERR, msg);
    }
    
    public UbfBTYPERRException(String msg, Throwable throwable) {
        super(AtmiConstants.BTYPERR, msg, throwable);
    }
}


