package org.endurox;

/**
 * BERFU1 exception
 */
public class UbfBERFU1Exception extends UbfException {
        
    public UbfBERFU1Exception(String msg) {
        super(msg, AtmiConstants.BERFU1);
    }
    
    public UbfBERFU1Exception(String s, Throwable throwable) {
        super(s, AtmiConstants.BERFU1, throwable);
    }
}


