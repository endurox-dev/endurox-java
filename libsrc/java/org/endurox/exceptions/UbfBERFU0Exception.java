package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * BERFU0 exception
 */
public class UbfBERFU0Exception extends UbfException {
        
    public UbfBERFU0Exception(String msg) {
        super(AtmiConst.BERFU0, msg);
    }
    
    public UbfBERFU0Exception(String msg, Throwable throwable) {
        super(AtmiConst.BERFU0, msg, throwable);
    }
}


