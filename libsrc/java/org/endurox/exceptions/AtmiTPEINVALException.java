package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * TPEINVAL exception
 */
public class AtmiTPEINVALException extends AtmiException {
        
    public AtmiTPEINVALException(String msg) {
        super(AtmiConst.TPEINVAL, msg);
    }
    
    public AtmiTPEINVALException(String msg, Throwable throwable) {
        super(AtmiConst.TPEINVAL, msg, throwable);
    }
}


