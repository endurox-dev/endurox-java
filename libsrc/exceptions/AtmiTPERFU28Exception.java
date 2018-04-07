package org.endurox;

/**
 * TPERFU28 exception
 */
public class AtmiTPERFU28Exception extends AtmiException {
        
    public AtmiTPERFU28Exception(String msg) {
        super(msg, AtmiConstants.TPERFU28);
    }
    
    public AtmiTPERFU28Exception(String s, Throwable throwable) {
        super(s, AtmiConstants.TPERFU28, throwable);
    }
}


