package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * NENOCONN exception
 */
public class NstdNENOCONNException extends NstdException {
        
    public NstdNENOCONNException(String msg) {
        super(AtmiConst.NENOCONN, msg);
    }
    
    public NstdNENOCONNException(String msg, Throwable throwable) {
        super(AtmiConst.NENOCONN, msg, throwable);
    }
}


