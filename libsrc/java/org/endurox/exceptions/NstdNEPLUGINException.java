package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * NEPLUGIN exception
 */
public class NstdNEPLUGINException extends NstdException {
        
    public NstdNEPLUGINException(String msg) {
        super(AtmiConstants.NEPLUGIN, msg);
    }
    
    public NstdNEPLUGINException(String msg, Throwable throwable) {
        super(AtmiConstants.NEPLUGIN, msg, throwable);
    }
}


