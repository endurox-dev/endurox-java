package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * BNOCNAME exception
 */
public class UbfBNOCNAMEException extends UbfException {
        
    public UbfBNOCNAMEException(String msg) {
        super(AtmiConstants.BNOCNAME, msg);
    }
    
    public UbfBNOCNAMEException(String msg, Throwable throwable) {
        super(AtmiConstants.BNOCNAME, msg, throwable);
    }
}


