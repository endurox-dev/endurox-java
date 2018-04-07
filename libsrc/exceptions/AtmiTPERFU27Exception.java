package org.endurox;

/**
 * TPERFU27 exception
 */
public class AtmiTPERFU27Exception extends AtmiException {
        
    public AtmiTPERFU27Exception(String msg) {
        super(msg, AtmiConstants.TPERFU27);
    }
    
    public AtmiTPERFU27Exception(String s, Throwable throwable) {
        super(s, AtmiConstants.TPERFU27, throwable);
    }
}


