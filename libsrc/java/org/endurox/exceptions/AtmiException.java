/**
 * @brief Generate exception code
 *
 * @class AtmiException
 */
/* -----------------------------------------------------------------------------
 * Enduro/X Middleware Platform for Distributed Transaction Processing
 * Copyright (C) 2009-2016, ATR Baltic, Ltd. All Rights Reserved.
 * Copyright (C) 2017-2019, Mavimax, Ltd. All Rights Reserved.
 * This software is released under one of the following licenses:
 * LGPL or Mavimax's license for commercial use.
 * See LICENSE text.
 *
 * C (as designed by Dennis Ritchie and later authors) language code is licensed
 * under Enduro/X Modified GNU Affero General Public License, version 3.
 * See LICENSE_C text.
 * -----------------------------------------------------------------------------
 * LGPL license:
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License, version 3 as published
 * by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU Lesser General Public License, version 3
 * for more details.
 *
 * You should have received a copy of the Lesser General Public License along 
 * with this program; if not, write to the Free Software Foundation, Inc., 
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * -----------------------------------------------------------------------------
 * A commercial use license is available from Mavimax, Ltd
 * contact@mavimax.com
 * -----------------------------------------------------------------------------
 */
package org.endurox.exceptions;
import org.endurox.AtmiConst;
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
    	
        case AtmiConst.TPEABORT:
            return new AtmiTPEABORTException(msg);

	    case AtmiConst.TPEBADDESC:
            return new AtmiTPEBADDESCException(msg);
	
	    case AtmiConst.TPEBLOCK:
            return new AtmiTPEBLOCKException(msg);

	    case AtmiConst.TPEINVAL:
            return new AtmiTPEINVALException(msg);
	
	    case AtmiConst.TPELIMIT:
            return new AtmiTPELIMITException(msg);
	
	    case AtmiConst.TPENOENT:
            return new AtmiTPENOENTException(msg);
	
	    case AtmiConst.TPEOS:
            return new AtmiTPEOSException(msg);
	
	    case AtmiConst.TPEPERM:
            return new AtmiTPEPERMException(msg);
	
	    case AtmiConst.TPEPROTO:
            return new AtmiTPEPROTOException(msg);
	
	    case AtmiConst.TPESVCERR:
            return new AtmiTPESVCERRException(msg);
	
	    case AtmiConst.TPESVCFAIL:
            return new AtmiTPESVCFAILException(msg);
	
	    case AtmiConst.TPESYSTEM:
            return new AtmiTPESYSTEMException(msg);
	
	    case AtmiConst.TPETIME:
            return new AtmiTPETIMEException(msg);
	
	    case AtmiConst.TPETRAN:
            return new AtmiTPETRANException(msg);
	
	    case AtmiConst.TPGOTSIG:
            return new AtmiTPGOTSIGException(msg);
	
	    case AtmiConst.TPERMERR:
            return new AtmiTPERMERRException(msg);
	
	    case AtmiConst.TPEITYPE:
            return new AtmiTPEITYPEException(msg);
	
	    case AtmiConst.TPEOTYPE:
            return new AtmiTPEOTYPEException(msg);
	
	    case AtmiConst.TPERELEASE:
            return new AtmiTPERELEASEException(msg);
	
	    case AtmiConst.TPEHAZARD:
            return new AtmiTPEHAZARDException(msg);
	
	    case AtmiConst.TPEHEURISTIC:
            return new AtmiTPEHEURISTICException(msg);
	
	    case AtmiConst.TPEEVENT:
            return new AtmiTPEEVENTException(msg);
	
	    case AtmiConst.TPEMATCH:
            return new AtmiTPEMATCHException(msg);
	
	    case AtmiConst.TPEDIAGNOSTIC:
            return new AtmiTPEDIAGNOSTICException(msg);
	
	    case AtmiConst.TPEMIB:
            return new AtmiTPEMIBException(msg);
	
	    case AtmiConst.TPERFU26:
            return new AtmiTPERFU26Exception(msg);
	
	    case AtmiConst.TPERFU27:
            return new AtmiTPERFU27Exception(msg);
	
	    case AtmiConst.TPERFU28:
            return new AtmiTPERFU28Exception(msg);
	
	    case AtmiConst.TPERFU29:
            return new AtmiTPERFU29Exception(msg);
	
	    case AtmiConst.TPINITFAIL:
            return new AtmiTPINITFAILException(msg);
    	}
    	//Return generic exception if not matched any code
    	return new AtmiException(tperrno, msg);
    }

}

/* vim: set ts=4 sw=4 et smartindent: */
