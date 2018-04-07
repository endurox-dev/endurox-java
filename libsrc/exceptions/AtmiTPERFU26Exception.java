package org.endurox;

/**
 * TPERFU26 exception
 */
public class AtmiTPERFU26Exception extends AtmiException {
        
    public AtmiTPERFU26Exception(String msg) {
        super(msg, AtmiConstants.TPERFU26);
    }
    
    public AtmiTPERFU26Exception(String s, Throwable throwable) {
        super(s, AtmiConstants.TPERFU26, throwable);
    }
}


