package org.endurox;

/**
 * TPERFU29 exception
 */
public class AtmiTPERFU29Exception extends AtmiException {
        
    public AtmiTPERFU29Exception(String msg) {
        super(msg, AtmiConstants.TPERFU29);
    }
    
    public AtmiTPERFU29Exception(String s, Throwable throwable) {
        super(s, AtmiConstants.TPERFU29, throwable);
    }
}


