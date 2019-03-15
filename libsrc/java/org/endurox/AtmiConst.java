/**
 * @brief Class holding list of ATMI Constants
 *
 * @class AtmiConst
 */
/* -----------------------------------------------------------------------------
 * Enduro/X Middleware Platform for Distributed Transaction Processing
 * Copyright (C) 2009-2016, ATR Baltic, Ltd. All Rights Reserved.
 * Copyright (C) 2017-2018, Mavimax, Ltd. All Rights Reserved.
 * This software is released under one of the following licenses:
 * AGPL or Mavimax's license for commercial use.
 * -----------------------------------------------------------------------------
 * AGPL license:
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Affero General Public License, version 3 as published
 * by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU Affero General Public License, version 3
 * for more details.
 *
 * You should have received a copy of the GNU Affero General Public License along 
 * with this program; if not, write to the Free Software Foundation, Inc., 
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 * -----------------------------------------------------------------------------
 * A commercial use license is available from Mavimax, Ltd
 * contact@mavimax.com
 * -----------------------------------------------------------------------------
 */
package org.endurox;

/**
 * ATMI constants
 */
public class AtmiConst {
    
    /**
     * ATMI Error codes
     * @defgroup atmierrors XATMI Error code
     * @{
     */
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
    /** Service error - service failed to generate response */
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
    
    /** @} */ // end of atmierrors

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
        
    /** Get log level for NDRXD        */
    public static final long TPLOGQI_GET_NDRX            = 0x00000001;
    /** Get log level for UBF        */
    public static final long TPLOGQI_GET_UBF             = 0x00000002;
    /** Get level for tp logger */
    public static final long TPLOGQI_GET_TP              = 0x00000004;
    /** Eval detailed flag  */
    public static final long TPLOGQI_EVAL_DETAILED       = 0x00000008;
    /** Return results (log level & detailed flag) event logging shall not be done */
    public static final long TPLOGQI_EVAL_RETURN         = 0x00000010;
    
    /** settings for ATMI logging        */
    public static final long  LOG_FACILITY_NDRX           = 0x00000001;
    /** settings for UBF logging         */
    public static final long  LOG_FACILITY_UBF            = 0x00000002;
    /** settings for TP logging          */
    public static final long  LOG_FACILITY_TP             = 0x00000004;
    /** settings for TP, thread based logging */
    public static final long  LOG_FACILITY_TP_THREAD      = 0x00000008;
    /** tp, Request logging, thread based*/
    public static final long  LOG_FACILITY_TP_REQUEST     = 0x00000010;
    /** ndrx thread logging              */
    public static final long  LOG_FACILITY_NDRX_THREAD    = 0x00000020;
    /** ubf thread logging               */
    public static final long  LOG_FACILITY_UBF_THREAD     = 0x00000040;
    /** ndrx request logging             */
    public static final long  LOG_FACILITY_NDRX_REQUEST   = 0x00000080;
    /** ubf request logging              */
    public static final long  LOG_FACILITY_UBF_REQUEST    = 0x00000100;

    /** Mask of the log facility bitwise flags */
    public static final long  LOG_FACILITY_MASK           = 0x0000ffff;

    /** first byte is reserved for log level (should not collide with LOG_FACILITY!) */
    public static final long  TPLOGQI_RET_HAVDETAILED     = 0x00010000;            
    /** Bit offset for log level in tpLogQInfo return */
    public static final long  TPLOGQI_RET_DBGLEVBITS      =  24;
    
    /** Log lever Always (unless debug disabled) */
    public static final int LOG_ALWAYS                  = 1;
    /** Error log level */
    public static final int LOG_ERROR                   = 2;
    /** Warning log level */
    public static final int LOG_WARN                    = 3;
    /** Informational log level */
    public static final int LOG_INFO                    = 4;
    /** Debug logs enable */
    public static final int LOG_DEBUG                   = 5;
    /** Maximum log level, including binary dumps */
    public static final int LOG_DUMP                    = 6;
    
    /** rval in tpreturn - Service failed */
    public static final int TPFAIL           = 0x00000001;
    /** rval in tpreturn - Service Succeed */
    public static final int TPSUCCESS       = 0x00000002;
    /** rval in tpreturn - RFU */
    public static final int TPEXIT          = 0x08000000;
    
    /**
     * ATMI flags
     * @defgroup atmiflags XATMI tpcall/tpreturn, etc... flags
     * @{
     */
    public static final int TPNOBLOCK       = 0x00000001;
    public static final int TPSIGRSTRT      = 0x00000002;
    public static final int TPNOREPLY       = 0x00000004;
    public static final int TPNOTRAN        = 0x00000008;
    public static final int TPTRAN          = 0x00000010;
    public static final int TPNOTIME        = 0x00000020;
    public static final int TPABSOLUTE      = 0x00000040;
    public static final int TPGETANY        = 0x00000080;
    public static final int TPNOCHANGE      = 0x00000100;
    public static final int TPCONV          = 0x00000400;
    public static final int TPSENDONLY      = 0x00000800;
    public static final int TPRECVONLY      = 0x00001000;
    public static final int TPACK           = 0x00002000;
    /** Software raised service error, any   */
    public static final int TPSOFTERR       = 0x00020000;
    /** Suspend current transaction          */
    public static final int TPTRANSUSPEND   = 0x00040000;
    /** Soft timout condition -> ret TPETIME */
    public static final int TPSOFTTIMEOUT   = 0x00080000;
    /** Simulate that service is not found   */
    public static final int TPSOFTENOENT    = 0x00100000;
    /** Don't restore autbuf in srv context  */
    public static final int TPNOAUTBUF      = 0x00200000;
    /** RFU, tux compatibility */
    public static final int RESERVED_BIT1   = 0x00400000;
    /** Use regular expressions for match    */
    public static final int TPREGEXMATCH    = 0x00800000;
    /** Do not lookup cache                  */
    public static final int TPNOCACHELOOK   = 0x01000000;
    /** Do not save data to cache            */
    public static final int TPNOCACHEADD    = 0x02000000;
    /** Do not use cached data               */
    public static final int TPNOCACHEDDATA  = 0x04000000;
    /** Do not abort global transaction, even if service failed */
    public static final int TPNOABORT       = 0x08000000;
            
    /** @} */ // end of atmiflags
    
    /**
     * Conversational IPC event flags
     * @defgroup cnvevents Conversational control constants
     * @{
     */
    
    /** 
     * Disconnect received from other conversation end, by issuing tpdiscon(). 
     * The global transaction in progress is aborted in this case. 
     */
    public static final int TPEV_DISCONIMM = 0x0001;
    
    /**
     * Server died and queue was removed.
     */
    public static final int TPEV_SVCERR    = 0x0002;
    
    /**
     * Server returned TPFAIL with tpreturn()
     */
    public static final int TPEV_SVCFAIL   = 0x0004;
    
    /**
     * Server did tpreturn() with TPSUCCESS.
     */
    public static final int TPEV_SVCSUCC   = 0x0008;
    
    /**
     * The sender program issued tpsend() with flag TPEV_SENDONLY, 
     * meaning that we become senders.
     */
    public static final int TPEV_SENDONLY  = 0x0020;
    /** @} */ // end of cnvevents
    
    
     /**
     * Enduro/X Queue control
     * @defgroup qctl Queue control constants
     * @{
     */
    
    /**
     * Queue correlator id, bin len
     */
    public static final int TMCORRIDLEN = 32;
    
    
    /**
     * Queueu message id len, bin
     */
    public static final int TMMSGIDLEN = 32;
    
    /** @} */ // end of qctl

}
/* vim: set ts=4 sw=4 et smartindent: */
