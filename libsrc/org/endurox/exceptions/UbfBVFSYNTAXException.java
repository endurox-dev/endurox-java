package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * BVFSYNTAX exception
 */
public class UbfBVFSYNTAXException extends UbfException {
        
    public UbfBVFSYNTAXException(String msg) {
        super(AtmiConstants.BVFSYNTAX, msg);
    }
    
    public UbfBVFSYNTAXException(String msg, Throwable throwable) {
        super(AtmiConstants.BVFSYNTAX, msg, throwable);
    }
}


