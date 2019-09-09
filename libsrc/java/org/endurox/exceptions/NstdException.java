/**
 * @brief Generate exception code
 *
 * @class NstdException
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

/* vim: set ts=4 sw=4 et smartindent: */
