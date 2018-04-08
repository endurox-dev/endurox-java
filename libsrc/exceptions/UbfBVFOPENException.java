package org.endurox;

/**
 * BVFOPEN exception
 */
public class UbfBVFOPENException extends UbfException {
        
    public UbfBVFOPENException(String msg) {
        super(AtmiConstants.BVFOPEN, msg);
    }
    
    public UbfBVFOPENException(String msg, Throwable throwable) {
        super(AtmiConstants.BVFOPEN, msg, throwable);
    }
}


