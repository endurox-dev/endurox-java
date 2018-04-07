package org.endurox;

/**
 * BALIGNERR exception
 */
public class UbfBALIGNERRException extends UbfException {
        
    public UbfBALIGNERRException(String msg) {
        super(msg, AtmiConstants.BALIGNERR);
    }
    
    public UbfBALIGNERRException(String s, Throwable throwable) {
        super(s, AtmiConstants.BALIGNERR, throwable);
    }
}


