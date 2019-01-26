package org.endurox.exceptions;
import org.endurox.AtmiConst;

/**
 * BNOCNAME exception
 */
public class UbfBNOCNAMEException extends UbfException {
        
    public UbfBNOCNAMEException(String msg) {
        super(AtmiConst.BNOCNAME, msg);
    }
    
    public UbfBNOCNAMEException(String msg, Throwable throwable) {
        super(AtmiConst.BNOCNAME, msg, throwable);
    }
}


