package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * BERFU0 exception
 */
public class UbfBERFU0Exception extends UbfException {
        
    public UbfBERFU0Exception(String msg) {
        super(AtmiConstants.BERFU0, msg);
    }
    
    public UbfBERFU0Exception(String msg, Throwable throwable) {
        super(AtmiConstants.BERFU0, msg, throwable);
    }
}


