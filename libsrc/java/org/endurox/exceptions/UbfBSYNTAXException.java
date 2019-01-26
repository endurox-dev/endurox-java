package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * BSYNTAX exception
 */
public class UbfBSYNTAXException extends UbfException {
        
    public UbfBSYNTAXException(String msg) {
        super(AtmiConst.BSYNTAX, msg);
    }
    
    public UbfBSYNTAXException(String msg, Throwable throwable) {
        super(AtmiConst.BSYNTAX, msg, throwable);
    }
}


