package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * TPEMIB exception
 */
public class AtmiTPEMIBException extends AtmiException {
        
    public AtmiTPEMIBException(String msg) {
        super(AtmiConst.TPEMIB, msg);
    }
    
    public AtmiTPEMIBException(String msg, Throwable throwable) {
        super(AtmiConst.TPEMIB, msg, throwable);
    }
}


