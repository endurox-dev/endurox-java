package org.endurox;

/**
 * ATMI Base exception
 */
public class AtmiException extends RuntimeException {
	
	/**
	 * Error code
	 */
	private int errorCode;
	
	/**
	 * Create ATMI Exception
	 * @param msg error message
	 * @param tperrno tp error code
	 */
    public AtmiException(String msg, int tperrno) {
        super(msg);
        
        errorCode = tperrno;
    }
    
    /**
     * ATMI Exception, with embedded error
     * @param s
     * @param tperrno
     * @param throwable
     */
    public AtmiException(String s, int tperrno, Throwable throwable) {
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

