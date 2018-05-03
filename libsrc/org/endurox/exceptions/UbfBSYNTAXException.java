package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * BSYNTAX exception
 */
public class UbfBSYNTAXException extends UbfException {
        
    public UbfBSYNTAXException(String msg) {
        super(AtmiConstants.BSYNTAX, msg);
    }
    
    public UbfBSYNTAXException(String msg, Throwable throwable) {
        super(AtmiConstants.BSYNTAX, msg, throwable);
    }
}


