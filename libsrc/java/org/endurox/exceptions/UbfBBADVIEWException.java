package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * BBADVIEW exception
 */
public class UbfBBADVIEWException extends UbfException {
        
    public UbfBBADVIEWException(String msg) {
        super(AtmiConstants.BBADVIEW, msg);
    }
    
    public UbfBBADVIEWException(String msg, Throwable throwable) {
        super(AtmiConstants.BBADVIEW, msg, throwable);
    }
}


