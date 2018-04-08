package org.endurox;

/**
 * BERFU1 exception
 */
public class UbfBERFU1Exception extends UbfException {
        
    public UbfBERFU1Exception(String msg) {
        super(AtmiConstants.BERFU1, msg);
    }
    
    public UbfBERFU1Exception(String msg, Throwable throwable) {
        super(AtmiConstants.BERFU1, msg, throwable);
    }
}


