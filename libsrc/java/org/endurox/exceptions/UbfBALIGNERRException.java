package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * BALIGNERR exception
 */
public class UbfBALIGNERRException extends UbfException {
        
    public UbfBALIGNERRException(String msg) {
        super(AtmiConst.BALIGNERR, msg);
    }
    
    public UbfBALIGNERRException(String msg, Throwable throwable) {
        super(AtmiConst.BALIGNERR, msg, throwable);
    }
}


