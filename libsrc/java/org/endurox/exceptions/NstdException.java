package org.endurox.exceptions;
import org.endurox.AtmiConst;

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
    	
	        case AtmiConst.NEINVALINI:
	            return new NstdNEINVALINIException(msg);
	
	        case AtmiConst.NEMALLOC:
	            return new NstdNEMALLOCException(msg);
	
	        case AtmiConst.NEUNIX:
	            return new NstdNEUNIXException(msg);
	
	        case AtmiConst.NEINVAL:
	            return new NstdNEINVALException(msg);
	
	        case AtmiConst.NESYSTEM:
	            return new NstdNESYSTEMException(msg);
	
	        case AtmiConst.NEMANDATORY:
	            return new NstdNEMANDATORYException(msg);
	
	        case AtmiConst.NEFORMAT:
	            return new NstdNEFORMATException(msg);
	
	        case AtmiConst.NETOUT:
	            return new NstdNETOUTException(msg);
	
	        case AtmiConst.NENOCONN:
	            return new NstdNENOCONNException(msg);
	
	        case AtmiConst.NELIMIT:
	            return new NstdNELIMITException(msg);
	
	        case AtmiConst.NEPLUGIN:
	            return new NstdNEPLUGINException(msg);
	
	        case AtmiConst.NENOSPACE:
	            return new NstdNENOSPACEException(msg);
	
	        case AtmiConst.NEINVALKEY:
	            return new NstdNEINVALKEYException(msg);
    	}
    	//Return generic exception if not matched any code
    	return new NstdException(tperrno, msg);
    
    }
}

