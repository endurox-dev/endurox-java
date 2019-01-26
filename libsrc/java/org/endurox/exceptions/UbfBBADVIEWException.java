package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * BBADVIEW exception
 */
public class UbfBBADVIEWException extends UbfException {
        
    public UbfBBADVIEWException(String msg) {
        super(AtmiConst.BBADVIEW, msg);
    }
    
    public UbfBBADVIEWException(String msg, Throwable throwable) {
        super(AtmiConst.BBADVIEW, msg, throwable);
    }
}


