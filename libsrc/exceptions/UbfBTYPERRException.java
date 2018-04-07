package org.endurox;

/**
 * BTYPERR exception
 */
public class UbfBTYPERRException extends UbfException {
        
    public UbfBTYPERRException(String msg) {
        super(msg, AtmiConstants.BTYPERR);
    }
    
    public UbfBTYPERRException(String s, Throwable throwable) {
        super(s, AtmiConstants.BTYPERR, throwable);
    }
}


