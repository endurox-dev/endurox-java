package org.endurox;

/**
 * BBADVIEW exception
 */
public class UbfBBADVIEWException extends UbfException {
        
    public UbfBBADVIEWException(String msg) {
        super(msg, AtmiConstants.BBADVIEW);
    }
    
    public UbfBBADVIEWException(String s, Throwable throwable) {
        super(s, AtmiConstants.BBADVIEW, throwable);
    }
}


