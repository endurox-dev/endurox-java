package org.endurox;

/**
 * TPEINVAL exception
 */
public class AtmiTPEINVALException extends AtmiException {
        
    public AtmiTPEINVALException(String msg) {
        super(msg, AtmiConstants.TPEINVAL);
    }
    
    public AtmiTPEINVALException(String s, Throwable throwable) {
        super(s, AtmiConstants.TPEINVAL, throwable);
    }
}


