package org.endurox;

/**
 * BBADNAME exception
 */
public class UbfBBADNAMEException extends UbfException {
        
    public UbfBBADNAMEException(String msg) {
        super(msg, AtmiConstants.BBADNAME);
    }
    
    public UbfBBADNAMEException(String s, Throwable throwable) {
        super(s, AtmiConstants.BBADNAME, throwable);
    }
}


