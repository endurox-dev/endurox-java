package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * BFTSYNTAX exception
 */
public class UbfBFTSYNTAXException extends UbfException {
        
    public UbfBFTSYNTAXException(String msg) {
        super(AtmiConstants.BFTSYNTAX, msg);
    }
    
    public UbfBFTSYNTAXException(String msg, Throwable throwable) {
        super(AtmiConstants.BFTSYNTAX, msg, throwable);
    }
}


