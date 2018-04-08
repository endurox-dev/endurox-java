package org.endurox;

/**
 * TPERFU29 exception
 */
public class AtmiTPERFU29Exception extends AtmiException {
        
    public AtmiTPERFU29Exception(String msg) {
        super(AtmiConstants.TPERFU29, msg);
    }
    
    public AtmiTPERFU29Exception(String msg, Throwable throwable) {
        super(AtmiConstants.TPERFU29, msg, throwable);
    }
}


