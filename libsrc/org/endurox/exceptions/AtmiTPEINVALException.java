package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * TPEINVAL exception
 */
public class AtmiTPEINVALException extends AtmiException {
        
    public AtmiTPEINVALException(String msg) {
        super(AtmiConstants.TPEINVAL, msg);
    }
    
    public AtmiTPEINVALException(String msg, Throwable throwable) {
        super(AtmiConstants.TPEINVAL, msg, throwable);
    }
}


