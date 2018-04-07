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
	 * @param msg error message
	 * @param tperrno tp error code
	 */
    public UbfException(String msg, int tperrno) {
        super(msg);
        
        errorCode = tperrno;
    }
    
    /**
     * ATMI Exception, with embedded error
     * @param s
     * @param tperrno
     * @param throwable
     */
    public UbfException(String s, int tperrno, Throwable throwable) {
        super(s, throwable);
        errorCode = tperrno;
        
    }
    
    /**
     * return error code
     * @return
     */
    public int getErrorCode() {
    	return errorCode;
    }
}

