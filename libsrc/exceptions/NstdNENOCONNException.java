package org.endurox;

/**
 * NENOCONN exception
 */
public class NstdNENOCONNException extends NstdException {
        
    public NstdNENOCONNException(String msg) {
        super(msg, AtmiConstants.NENOCONN);
    }
    
    public NstdNENOCONNException(String s, Throwable throwable) {
        super(s, AtmiConstants.NENOCONN, throwable);
    }
}


