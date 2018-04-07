package org.endurox;

/**
 * BBADACM exception
 */
public class UbfBBADACMException extends UbfException {
        
    public UbfBBADACMException(String msg) {
        super(msg, AtmiConstants.BBADACM);
    }
    
    public UbfBBADACMException(String s, Throwable throwable) {
        super(s, AtmiConstants.BBADACM, throwable);
    }
}


