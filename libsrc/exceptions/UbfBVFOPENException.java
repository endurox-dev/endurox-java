package org.endurox;

/**
 * BVFOPEN exception
 */
public class UbfBVFOPENException extends UbfException {
        
    public UbfBVFOPENException(String msg) {
        super(msg, AtmiConstants.BVFOPEN);
    }
    
    public UbfBVFOPENException(String s, Throwable throwable) {
        super(s, AtmiConstants.BVFOPEN, throwable);
    }
}


