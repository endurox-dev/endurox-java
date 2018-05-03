package org.endurox.exceptions;
import org.endurox.AtmiConstants;

/**
 * ATMI Base exception
 */
public class NstdException extends RuntimeException {
	
    /**
     * Error code
     */
    private int errorCode;

    /**
     * Create ATMI Exception
     * @param msg error message
     * @param tperrno tp error code
     */
    public NstdException(int tperrno, String msg) {
        super(msg);
        
        errorCode = tperrno;
    }
    
    /**
     * ATMI Exception, with embedded error
     * @param tperrno error number
     * @param msg error message
     * @param throwable
     */
    public NstdException(int tperrno, String msg, Throwable throwable) {
        super(msg, throwable);
        errorCode = tperrno;
        
    }
    
    /**
     * return error code
     * @return
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
    public static NstdException create(int tperrno, String msg) {
    	
    	switch (tperrno) {
    	
	        case AtmiConstants.NEINVALINI:
	            return new NstdNEINVALINIException(msg);
	
	        case AtmiConstants.NEMALLOC:
	            return new NstdNEMALLOCException(msg);
	
	        case AtmiConstants.NEUNIX:
	            return new NstdNEUNIXException(msg);
	
	        case AtmiConstants.NEINVAL:
	            return new NstdNEINVALException(msg);
	
	        case AtmiConstants.NESYSTEM:
	            return new NstdNESYSTEMException(msg);
	
	        case AtmiConstants.NEMANDATORY:
	            return new NstdNEMANDATORYException(msg);
	
	        case AtmiConstants.NEFORMAT:
	            return new NstdNEFORMATException(msg);
	
	        case AtmiConstants.NETOUT:
	            return new NstdNETOUTException(msg);
	
	        case AtmiConstants.NENOCONN:
	            return new NstdNENOCONNException(msg);
	
	        case AtmiConstants.NELIMIT:
	            return new NstdNELIMITException(msg);
	
	        case AtmiConstants.NEPLUGIN:
	            return new NstdNEPLUGINException(msg);
	
	        case AtmiConstants.NENOSPACE:
	            return new NstdNENOSPACEException(msg);
	
	        case AtmiConstants.NEINVALKEY:
	            return new NstdNEINVALKEYException(msg);
    	}
    	//Return generic exception if not matched any code
    	return new NstdException(tperrno, msg);
    
    }
}

