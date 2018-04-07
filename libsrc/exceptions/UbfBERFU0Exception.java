package org.endurox;

/**
 * BERFU0 exception
 */
public class UbfBERFU0Exception extends UbfException {
        
    public UbfBERFU0Exception(String msg) {
        super(msg, AtmiConstants.BERFU0);
    }
    
    public UbfBERFU0Exception(String s, Throwable throwable) {
        super(s, AtmiConstants.BERFU0, throwable);
    }
}


