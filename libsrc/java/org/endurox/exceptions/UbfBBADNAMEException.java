package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * BBADNAME exception
 */
public class UbfBBADNAMEException extends UbfException {
        
    public UbfBBADNAMEException(String msg) {
        super(AtmiConst.BBADNAME, msg);
    }
    
    public UbfBBADNAMEException(String msg, Throwable throwable) {
        super(AtmiConst.BBADNAME, msg, throwable);
    }
}


