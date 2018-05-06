package org.endurox;
/**
 * @class AtmiConstants
 *
 * @brief Class holding list of ATMI Constants
 */
/* -----------------------------------------------------------------------------
 * Enduro/X Middleware Platform for Distributed Transaction Processing
 * Copyright (C) 2018, Mavimax, Ltd. All Rights Reserved.
 * This software is released under one of the following licenses:
 * GPL or Mavimax's license for commercial use.
 * -----------------------------------------------------------------------------
 * GPL license:
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * -----------------------------------------------------------------------------
 * A commercial use license is available from Mavimax, Ltd
 * contact@mavimax.com
 * -----------------------------------------------------------------------------
 */
public class AtmiConstants {
    /** Transaction is being aborted*/
    public static final int TPEABORT	= 1;
    /** Bad call descriptor */
    public static final int TPEBADDESC	= 2;
    /** Blocking condition occurred */
    public static final int TPEBLOCK	= 3;
    /** Invalid arguments passed */
    public static final int TPEINVAL	= 4;
    public static final int TPELIMIT	= 5;
    public static final int TPENOENT	= 6;
    public static final int TPEOS	= 7;
    public static final int TPEPERM	= 8;
    public static final int TPEPROTO	= 9;
    public static final int TPESVCERR	= 10;
    public static final int TPESVCFAIL	= 11;
    public static final int TPESYSTEM	= 12;
    public static final int TPETIME	= 13;
    public static final int TPETRAN	= 14;
    public static final int TPGOTSIG	= 15;
    public static final int TPERMERR	= 16;
    public static final int TPEITYPE	= 17;
    public static final int TPEOTYPE	= 18;
    public static final int TPERELEASE	= 19;
    public static final int TPEHAZARD	= 20;
    public static final int TPEHEURISTIC = 21;
    public static final int TPEEVENT	= 22;
    public static final int TPEMATCH	= 23;
    public static final int TPEDIAGNOSTIC = 24;
    public static final int TPEMIB	= 25;
    public static final int TPERFU26    = 26;
    public static final int TPERFU27	= 27;
    public static final int TPERFU28	= 28;
    public static final int TPERFU29	= 29;
    public static final int TPINITFAIL	= 30;

    /* UBF library error codes: */
    public static final int BERFU0	= 1;
    public static final int BALIGNERR	= 2;
    public static final int BNOTFLD	= 3;
    public static final int BNOSPACE	= 4;
    public static final int BNOTPRES	= 5;
    public static final int BBADFLD	= 6;
    public static final int BTYPERR	= 7;
    public static final int BEUNIX	= 8;
    public static final int BBADNAME	= 9;
    public static final int BMALLOC	= 10;
    public static final int BSYNTAX	= 11;
    public static final int BFTOPEN	= 12;
    public static final int BFTSYNTAX	= 13;
    public static final int BEINVAL	= 14;
    public static final int BERFU1	= 15;
    public static final int BBADTBL	= 16;
    public static final int BBADVIEW	= 17;
    public static final int BVFSYNTAX	= 18;
    public static final int BVFOPEN	= 19;
    public static final int BBADACM	= 20;
    public static final int BNOCNAME	= 21;
    public static final int BEBADOP	= 22;

    /* Enduro/X Standard library error codes: */

    /** Invalid INI file */
    public static final int NEINVALINI	= 1;
    /** Malloc failed */
    public static final int NEMALLOC	= 2;
    /** Unix error occurred */
    public static final int NEUNIX	= 3;
    /** Invalid value passed to function */
    public static final int NEINVAL	= 4;
    /** System failure */
    public static final int NESYSTEM	= 5;
    /** Mandatory field is missing */
    public static final int NEMANDATORY	= 6;
    /** Format error */
    public static final int NEFORMAT	= 7;
    /** Time-out condition */
    public static final int NETOUT	= 8;
    /** Connection not found */
    public static final int NENOCONN	= 9;
    /** Limit reached */
    public static final int NELIMIT	= 10;
    /** Plugin error */
    public static final int NEPLUGIN	= 11;
    /** No space */
    public static final int NENOSPACE	= 12;
    /** Invalid key (probably) */
    public static final int NEINVALKEY	= 13;

    /** Failed status */
    public static final int FAIL        = -1;
    
    /** Succeed status */
    public static final int SUCCEED     = 0;
    
}
