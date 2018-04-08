package org.endurox;

/**
 * BALIGNERR exception
 */
public class UbfBALIGNERRException extends UbfException {
        
    public UbfBALIGNERRException(String msg) {
        super(AtmiConstants.BALIGNERR, msg);
    }
    
    public UbfBALIGNERRException(String msg, Throwable throwable) {
        super(AtmiConstants.BALIGNERR, msg, throwable);
    }
}


