package org.endurox;

/**
 * BVFSYNTAX exception
 */
public class UbfBVFSYNTAXException extends UbfException {
        
    public UbfBVFSYNTAXException(String msg) {
        super(msg, AtmiConstants.BVFSYNTAX);
    }
    
    public UbfBVFSYNTAXException(String s, Throwable throwable) {
        super(s, AtmiConstants.BVFSYNTAX, throwable);
    }
}


