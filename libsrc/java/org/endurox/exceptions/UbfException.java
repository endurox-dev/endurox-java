/**
 * @brief Generate exception code
 *
 * @class UbfException
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
    public static UbfException create(int tperrno, String msg) {
    	
    	switch (tperrno) {
        case AtmiConst.BERFU0:
                        return new UbfBERFU0Exception(msg);

            case AtmiConst.BALIGNERR:
            return new UbfBALIGNERRException(msg);

            case AtmiConst.BNOTFLD:
            return new UbfBNOTFLDException(msg);

            case AtmiConst.BNOSPACE:
            return new UbfBNOSPACEException(msg);

            case AtmiConst.BNOTPRES:
            return new UbfBNOTPRESException(msg);

            case AtmiConst.BBADFLD:
            return new UbfBBADFLDException(msg);

            case AtmiConst.BTYPERR:
            return new UbfBTYPERRException(msg);

            case AtmiConst.BEUNIX:
            return new UbfBEUNIXException(msg);

            case AtmiConst.BBADNAME:
            return new UbfBBADNAMEException(msg);

            case AtmiConst.BMALLOC:
            return new UbfBMALLOCException(msg);

            case AtmiConst.BSYNTAX:
            return new UbfBSYNTAXException(msg);

            case AtmiConst.BFTOPEN:
            return new UbfBFTOPENException(msg);

            case AtmiConst.BFTSYNTAX:
            return new UbfBFTSYNTAXException(msg);

            case AtmiConst.BEINVAL:
            return new UbfBEINVALException(msg);

            case AtmiConst.BERFU1:
            return new UbfBERFU1Exception(msg);

            case AtmiConst.BBADTBL:
            return new UbfBBADTBLException(msg);

            case AtmiConst.BBADVIEW:
            return new UbfBBADVIEWException(msg);

            case AtmiConst.BVFSYNTAX:
            return new UbfBVFSYNTAXException(msg);

            case AtmiConst.BVFOPEN:
            return new UbfBVFOPENException(msg);

            case AtmiConst.BBADACM:
            return new UbfBBADACMException(msg);

            case AtmiConst.BNOCNAME:
            return new UbfBNOCNAMEException(msg);

            case AtmiConst.BEBADOP:
            return new UbfBEBADOPException(msg);

    	}
    	//Return generic exception if not matched any code
    	return new UbfException(tperrno, msg);
    }
}

/* vim: set ts=4 sw=4 et smartindent: */
