package org.endurox;

/**
 * BNOCNAME exception
 */
public class UbfBNOCNAMEException extends UbfException {
        
    public UbfBNOCNAMEException(String msg) {
        super(msg, AtmiConstants.BNOCNAME);
    }
    
    public UbfBNOCNAMEException(String s, Throwable throwable) {
        super(s, AtmiConstants.BNOCNAME, throwable);
    }
}


