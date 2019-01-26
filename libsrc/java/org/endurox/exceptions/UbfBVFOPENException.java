package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * BVFOPEN exception
 */
public class UbfBVFOPENException extends UbfException {
        
    public UbfBVFOPENException(String msg) {
        super(AtmiConst.BVFOPEN, msg);
    }
    
    public UbfBVFOPENException(String msg, Throwable throwable) {
        super(AtmiConst.BVFOPEN, msg, throwable);
    }
}


