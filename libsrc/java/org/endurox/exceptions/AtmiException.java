package org.endurox.exceptions;
import org.endurox.AtmiConstants;
import org.endurox.TypedBuffer;

/**
 * ATMI Base exception
 */
public class AtmiException extends RuntimeException {

    /**
     * reference to typed buffer (if any, can be set for tpcall results)
     */
    TypedBuffer data;

    /**
     * Get the reply buffer (if any)
     * @return return reply buffer from tpcall
     */
    public TypedBuffer getReplyBuffer() {
        return data;
    }
    
    /**
     * Error code
     */
    int errorCode;
	
    /**
     * Create ATMI Exception
     * @param tperrno tp error code
     * @param msg error message
     */
    public AtmiException(int tperrno, String msg) {
        super(msg);
        
        errorCode = tperrno;
    }
    
    /**
     * ATMI Exception, with embedded error
     * @param tperrno
     * @param s
     * @param throwable
     */
    public AtmiException(int tperrno, String msg, Throwable throwable) {
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
    public static AtmiException create(int tperrno, String msg) {
    	
    	switch (tperrno) {
    	
        case AtmiConstants.TPEABORT:
            return new AtmiTPEABORTException(msg);

	    case AtmiConstants.TPEBADDESC:
            return new AtmiTPEBADDESCException(msg);
	
	    case AtmiConstants.TPEBLOCK:
            return new AtmiTPEBLOCKException(msg);

	    case AtmiConstants.TPEINVAL:
            return new AtmiTPEINVALException(msg);
	
	    case AtmiConstants.TPELIMIT:
            return new AtmiTPELIMITException(msg);
	
	    case AtmiConstants.TPENOENT:
            return new AtmiTPENOENTException(msg);
	
	    case AtmiConstants.TPEOS:
            return new AtmiTPEOSException(msg);
	
	    case AtmiConstants.TPEPERM:
            return new AtmiTPEPERMException(msg);
	
	    case AtmiConstants.TPEPROTO:
            return new AtmiTPEPROTOException(msg);
	
	    case AtmiConstants.TPESVCERR:
            return new AtmiTPESVCERRException(msg);
	
	    case AtmiConstants.TPESVCFAIL:
            return new AtmiTPESVCFAILException(msg);
	
	    case AtmiConstants.TPESYSTEM:
            return new AtmiTPESYSTEMException(msg);
	
	    case AtmiConstants.TPETIME:
            return new AtmiTPETIMEException(msg);
	
	    case AtmiConstants.TPETRAN:
            return new AtmiTPETRANException(msg);
	
	    case AtmiConstants.TPGOTSIG:
            return new AtmiTPGOTSIGException(msg);
	
	    case AtmiConstants.TPERMERR:
            return new AtmiTPERMERRException(msg);
	
	    case AtmiConstants.TPEITYPE:
            return new AtmiTPEITYPEException(msg);
	
	    case AtmiConstants.TPEOTYPE:
            return new AtmiTPEOTYPEException(msg);
	
	    case AtmiConstants.TPERELEASE:
            return new AtmiTPERELEASEException(msg);
	
	    case AtmiConstants.TPEHAZARD:
            return new AtmiTPEHAZARDException(msg);
	
	    case AtmiConstants.TPEHEURISTIC:
            return new AtmiTPEHEURISTICException(msg);
	
	    case AtmiConstants.TPEEVENT:
            return new AtmiTPEEVENTException(msg);
	
	    case AtmiConstants.TPEMATCH:
            return new AtmiTPEMATCHException(msg);
	
	    case AtmiConstants.TPEDIAGNOSTIC:
            return new AtmiTPEDIAGNOSTICException(msg);
	
	    case AtmiConstants.TPEMIB:
            return new AtmiTPEMIBException(msg);
	
	    case AtmiConstants.TPERFU26:
            return new AtmiTPERFU26Exception(msg);
	
	    case AtmiConstants.TPERFU27:
            return new AtmiTPERFU27Exception(msg);
	
	    case AtmiConstants.TPERFU28:
            return new AtmiTPERFU28Exception(msg);
	
	    case AtmiConstants.TPERFU29:
            return new AtmiTPERFU29Exception(msg);
	
	    case AtmiConstants.TPINITFAIL:
            return new AtmiTPINITFAILException(msg);
    	}
    	//Return generic exception if not matched any code
    	return new AtmiException(tperrno, msg);
    }

}

