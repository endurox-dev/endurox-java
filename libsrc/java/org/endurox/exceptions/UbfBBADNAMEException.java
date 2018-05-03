package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * BBADNAME exception
 */
public class UbfBBADNAMEException extends UbfException {
        
    public UbfBBADNAMEException(String msg) {
        super(AtmiConstants.BBADNAME, msg);
    }
    
    public UbfBBADNAMEException(String msg, Throwable throwable) {
        super(AtmiConstants.BBADNAME, msg, throwable);
    }
}


