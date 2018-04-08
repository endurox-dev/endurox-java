package org.endurox;

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


