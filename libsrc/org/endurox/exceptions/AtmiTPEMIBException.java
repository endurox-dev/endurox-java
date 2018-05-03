package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * TPEMIB exception
 */
public class AtmiTPEMIBException extends AtmiException {
        
    public AtmiTPEMIBException(String msg) {
        super(AtmiConstants.TPEMIB, msg);
    }
    
    public AtmiTPEMIBException(String msg, Throwable throwable) {
        super(AtmiConstants.TPEMIB, msg, throwable);
    }
}


