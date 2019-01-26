package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * BVFSYNTAX exception
 */
public class UbfBVFSYNTAXException extends UbfException {
        
    public UbfBVFSYNTAXException(String msg) {
        super(AtmiConst.BVFSYNTAX, msg);
    }
    
    public UbfBVFSYNTAXException(String msg, Throwable throwable) {
        super(AtmiConst.BVFSYNTAX, msg, throwable);
    }
}


