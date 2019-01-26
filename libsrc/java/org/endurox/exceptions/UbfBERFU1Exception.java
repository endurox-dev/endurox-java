package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * BERFU1 exception
 */
public class UbfBERFU1Exception extends UbfException {
        
    public UbfBERFU1Exception(String msg) {
        super(AtmiConst.BERFU1, msg);
    }
    
    public UbfBERFU1Exception(String msg, Throwable throwable) {
        super(AtmiConst.BERFU1, msg, throwable);
    }
}


