package org.endurox;

/**
 * TPEMIB exception
 */
public class AtmiTPEMIBException extends AtmiException {
        
    public AtmiTPEMIBException(String msg) {
        super(msg, AtmiConstants.TPEMIB);
    }
    
    public AtmiTPEMIBException(String s, Throwable throwable) {
        super(s, AtmiConstants.TPEMIB, throwable);
    }
}


