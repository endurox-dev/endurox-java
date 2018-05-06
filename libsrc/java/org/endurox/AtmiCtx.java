package org.endurox;

import java.util.HashMap;
import java.util.Map;
import org.endurox.exceptions.AtmiTPESYSTEMException;

/**
 * @class AtmiCtx
 *
 * @brief Enduro/X ATMI Context Class
 *  This is main class used for almost all operations with Enduro/X middleware
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

public class AtmiCtx {
	
    /**
     * Pointer to C ATMI Context object
     */
    private long ctx = 0;

    static {
       System.loadLibrary("exjava"); // Load native library at runtime
                            // hello.dll (Windows) or libenduroxjava.so (Unixes)
    }
    
    /* Have some static hash list of the services we advertise */
    
    /**
     * Internal list of services
     */
    private static Map<String, Service> svcMap = new HashMap<String, Service>();
    
    /**
     * Server interface (if we run in server mode)
     */
    private static Server svr = null;  
    
    /* TODO: We need a registry with non terminated ATMI contexts
     * so that we can hook up the JVM and remove all open contexts at
     * shutdown 
     */

    /**
     * Get ATMI Error 
     * @return Error tuple (code and message)
     */
    private native ErrorTuple getAtmiError();

    /**
     * Allocate new ATMI Context
     * ptr to ATMI context
     * @return C pointer to context object
     * @exception AtmiTPESYSTEMException
     */
    private static native long tpnewctxt();

    /**
     * Free up ATMI Context
     * @param ctx ATMI Context
     */
    private static native void tpfreectxt(long ctx);

    /**
     * Allocate buffer
     * @param btype buffer type name
     * @param bsubtype buffer sub type or empty string (if no subtype)
     * @param size buffer size in bytes
     * @return allocate ATMI buffer
     * @throw AtmiTPEINVALException invalid arguments passed to {@code btype} or 
     *  {@code bsubtype}
     * @throw AtmiTPEOTYPEException invalid types specified (or sub-type)
     * @throw AtmiTPESYSTEMException system exception occurred
     * @throw AtmiTPEOSException Operating System error occurred
     */
    public native AtmiBuf tpAlloc(String btype, String bsubtype, long size);

    /**
     * Allocate new ATMI Context
     * @throw AtmiTPESYSTEMException Failed to allocate new context object in
     *  C space.
     */
    public AtmiCtx()  {
        // This thorws TPESYSTEM if failed.
       ctx = tpnewctxt();
    }
     
    /**
     * Terminate the ATMI Context
     * @throws Throwable 
     */
    @Override
    protected void finalize() throws Throwable {

       if (0x0 != ctx)
       {
          tpfreectxt(ctx);
       }

       //Remove ATMI context...
       super.finalize();
    }
    
    
    /**
     * Run server instance. Only one thread is allowed to step into this
     * @param svr 
     */
    public synchronized void TpRun(Server svr)
    {
        this.svr = svr;
        
        /* TODO: Call native server entry... */
    }
    
    // Test Driver
    public static void main(String[] args) {
     AtmiCtx ctx = new AtmiCtx();

     TypedUbf b = (TypedUbf)ctx.tpAlloc("UBF", "", 1024);

     System.out.printf("Buffer OK!");

     b = null;

     System.gc();

     ErrorTuple err = ctx.getAtmiError();
       System.out.printf("%d: %s\n", err.err, err.msg);

    }
    
    /* TODO: Have some loggers! */
    
    /**
     * Native logger call
     * @param lev log level
     * @param file optional file name
     * @param line optional line number in file (if no metadata infos, use -1)
     * @param message log message
     */
    private native void tpLogC(int lev, String file, long line, String message);
    
    /**
     * Get logger levels
     * So bit layout: RRRRRRRRnnnnNNNNuuuuUUUUttttTTTT
     * where n - flags (see LOGFLAGS_) for NDRX logger, N - log level of NDRX
     * where u - flags (see LOGFLAGS_) for UBF logger, U - log level of UBF
     * where t - flags (see LOGFLAGS_) for TP logger, T - log level of TP
     * @return oldest byte is reserved, then ndrx (4 bits flags, 4 bits lev), ubf, tp
     */
    private native int tpLogGetLevel();
    
    /**
     * Write the user log
     * @param lev Log level
     * @param format format string
     * @param arguments  format arguments
     */
    public void tpLog(int lev, String format, Object... arguments) {
        
        /* todo: detect log level */
        
        int log_config = tpLogGetLevel();
        
        /*  */
        if ( lev > (log_config & 0x0f) ) {
            /* nothing to do */
            return;
        }
        
        /* write the log according to the detail level with or with out
         * stack tracking
         */
        tpLogC(lev, "", AtmiConstants.FAIL, String.format(format, arguments));
       
    }
    
    
}

