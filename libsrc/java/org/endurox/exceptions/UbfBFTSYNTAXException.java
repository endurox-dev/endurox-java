package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * BFTSYNTAX exception
 */
public class UbfBFTSYNTAXException extends UbfException {
        
    public UbfBFTSYNTAXException(String msg) {
        super(AtmiConst.BFTSYNTAX, msg);
    }
    
    public UbfBFTSYNTAXException(String msg, Throwable throwable) {
        super(AtmiConst.BFTSYNTAX, msg, throwable);
    }
}


