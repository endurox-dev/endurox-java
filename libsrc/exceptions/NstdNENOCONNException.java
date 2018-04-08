package org.endurox;

/**
 * NENOCONN exception
 */
public class NstdNENOCONNException extends NstdException {
        
    public NstdNENOCONNException(String msg) {
        super(AtmiConstants.NENOCONN, msg);
    }
    
    public NstdNENOCONNException(String msg, Throwable throwable) {
        super(AtmiConstants.NENOCONN, msg, throwable);
    }
}


