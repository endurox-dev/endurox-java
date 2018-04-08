package org.endurox;

/**
 * ATMI Base exception
 */
public class UbfException extends RuntimeException {
	
	/**
	 * Error code
	 */
	private int errorCode;
	
	/**
	 * Create ATMI Exception
	 * @param tperrno tp error code
	 * @param msg error message
	 */
    public UbfException(int tperrno, String msg) {
        super(msg);
        
        errorCode = tperrno;
    }
    
    /**
     * ATMI Exception, with embedded error
     * @param tperrno error number
     * @param msg error message
     * @param throwable
     */
    public UbfException(int tperrno, String msg, Throwable throwable) {
        super(msg, throwable);
        errorCode = tperrno;
    }
    
    /**
     * return error code
     * @return UBF error code (see BE* error codes)
     */
    public int getErrorCode() {
    	return errorCode;
    }
    
    /**
     * Create exception (exact instance)
     * @param tperrno error code
     * @param msg error message
     * @return
     */
    public UbfException create(int tperrno, String msg) {
    	
    	switch (tperrno) {
    		case AtmiConstants.BERFU0:
				return new UbfBERFU0Exception(msg);

		    case AtmiConstants.BALIGNERR:
	            return new UbfBALIGNERRException(msg);
		
		    case AtmiConstants.BNOTFLD:
	            return new UbfBNOTFLDException(msg);
		
		    case AtmiConstants.BNOSPACE:
	            return new UbfBNOSPACEException(msg);
		
		    case AtmiConstants.BNOTPRES:
	            return new UbfBNOTPRESException(msg);
		
		    case AtmiConstants.BBADFLD:
	            return new UbfBBADFLDException(msg);
		
		    case AtmiConstants.BTYPERR:
	            return new UbfBTYPERRException(msg);
		
		    case AtmiConstants.BEUNIX:
	            return new UbfBEUNIXException(msg);
		
		    case AtmiConstants.BBADNAME:
	            return new UbfBBADNAMEException(msg);
		
		    case AtmiConstants.BMALLOC:
	            return new UbfBMALLOCException(msg);
		
		    case AtmiConstants.BSYNTAX:
	            return new UbfBSYNTAXException(msg);
		
		    case AtmiConstants.BFTOPEN:
	            return new UbfBFTOPENException(msg);
		
		    case AtmiConstants.BFTSYNTAX:
	            return new UbfBFTSYNTAXException(msg);
		
		    case AtmiConstants.BEINVAL:
	            return new UbfBEINVALException(msg);
		
		    case AtmiConstants.BERFU1:
	            return new UbfBERFU1Exception(msg);
		
		    case AtmiConstants.BBADTBL:
	            return new UbfBBADTBLException(msg);
		
		    case AtmiConstants.BBADVIEW:
	            return new UbfBBADVIEWException(msg);
		
		    case AtmiConstants.BVFSYNTAX:
	            return new UbfBVFSYNTAXException(msg);
		
		    case AtmiConstants.BVFOPEN:
	            return new UbfBVFOPENException(msg);
		
		    case AtmiConstants.BBADACM:
	            return new UbfBBADACMException(msg);
		
		    case AtmiConstants.BNOCNAME:
	            return new UbfBNOCNAMEException(msg);
		
		    case AtmiConstants.BEBADOP:
	            return new UbfBEBADOPException(msg);

    	}
    	//Return generic exception if not matched any code
    	return new UbfException(tperrno, msg);
    }
}

