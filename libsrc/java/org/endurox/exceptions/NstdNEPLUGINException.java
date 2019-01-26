package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * NEPLUGIN exception
 */
public class NstdNEPLUGINException extends NstdException {
        
    public NstdNEPLUGINException(String msg) {
        super(AtmiConst.NEPLUGIN, msg);
    }
    
    public NstdNEPLUGINException(String msg, Throwable throwable) {
        super(AtmiConst.NEPLUGIN, msg, throwable);
    }
}


