/**
 * @brief Enduro/X ATMI Context Class
 *   This is main class used for almost all operations with Enduro/X middleware
 *
 * @class AtmiCtx
 */
/* -----------------------------------------------------------------------------
 * Enduro/X Middleware Platform for Distributed Transaction Processing
 * Copyright (C) 2009-2016, ATR Baltic, Ltd. All Rights Reserved.
 * Copyright (C) 2017-2018, Mavimax, Ltd. All Rights Reserved.
 * This software is released under one of the following licenses:
 * GPL or Mavimax's license for commercial use.
 * -----------------------------------------------------------------------------
 * GPL license:
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 3 of the License, or (at your option) any later
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
package org.endurox;

import java.io.OutputStream;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import org.endurox.exceptions.AtmiTPEINVALException;
import org.endurox.exceptions.AtmiTPESYSTEMException;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
import org.endurox.exceptions.AtmiTPEOSException;

/*! @mainpage Enduro/X Programming main page
 *
 * @section standard_sec Programming standard page
 *
 * This section lists API practices used by Enduro/X Java package
 *
 * @subsection atmi_api_sec ATMI API
 * ATMI API uses lower case function names (instead of Java camel case). This
 * is for clener compatibility with C XATMI API. The same goes with the constants.
 * ATMI constants goes in upper case as in the C side.
 * 
 * Class names are typed as with Java Standard.
 *
 * @subsection java_api_sec Java Specific API
 * Java Specific APIs are using standard camel cases method names.
 * @subsection garbage_collection Java Specific API
 * All classes which are linked with C resources have the finalize() method
 * overriden, but due mostly undefined logic of GC's finalize() invocation
 * frequency, Enduro/X Java API implements cleanup() method, so that in code
 * explicitly resources could be free'd up.
 *
 */
public class AtmiCtx {
	
    /**
     * Pointer to C ATMI Context object
     */
    long ctx = 0;

    /**
     * Get The C Context
     * @return 
     */
    public long getCtx() {
        return ctx;
    }

    /**
     * Set the C context
     * @param ctx 
     */
    public void setCtx(long ctx) {
        this.ctx = ctx;
    }

    static {
       System.loadLibrary("exjava"); // Load native library at runtime
                            // *.dll (Windows) or *.so (Unixes)
    }
    
    /* Have some static hash list of the services we advertise */
    
    /**
     * Internal list of services
     */
    private static Map<String, Service> svcMap = new HashMap<String, Service>();
    
    /**
     * List of ATMI Contexts currently open
     */
    private static Map<Long, Long> ctxMap = new HashMap<Long, Long>();
    
    /**
     * Context map mutex
     */
    private static final Lock ctxMapMutex = new ReentrantLock(true);
    
    
    /**
     * Server interface (if we run in server mode)
     */
    private Server svr = null;  
    
    /**
     * Get server object (if any)
     * @return 
     */
    public Server getSvr() {
        return svr;
    }
    
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
    public native TypedBuffer tpalloc(String btype, String bsubtype, long size);

    /**
     * Allocate new ATMI Context
     * @throw AtmiTPESYSTEMException Failed to allocate new context object in
     *  C space.
     */
    public AtmiCtx()  {
        /* This thorws TPESYSTEM if failed.*/
       ctx = tpnewctxt();

        /* Add context to static synced list of context
         * this contexts shall be terminated when JVM stops.
         */
        if (0x0 != ctx)
        {
            // register this context in hash list for free up...
            ctxMapMutex.lock();
            try {
                //Hmm seems this does not allow to perform GC on object...
                //as we are in the list...
                ctxMap.put((Long)ctx, (Long)ctx);
            }
            finally {
                ctxMapMutex.unlock();
            }
        }
    }
    
    /**
     * Terminate XATMI Session. This does not remove the context.
     * To remove context (including XATMI terminate), use \ref cleanup() method.
     * @throws AtmiTPEPROTOException Called from XATMI server (main thread)
     * @throws AtmiTPESYSTEMException Failed to close conversations
     * @throws AtmiTPEOSException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info. 
     *  That could insufficient memory or other error
     */
    public native void tpterm();
    
    /**
     * Clean up the object (basically this is destructor) as we cannot relay
     * on finalize() auto call of from the Garbage Collector. Thus we object
     * goes out of the scope, this method needs to be called. This automatically
     * invokes \ref tpterm() too.
     */
    public void cleanup() {
        if (0x0 != ctx)
        {
            /* clean up the hash */
            ctxMapMutex.lock();
            try {
                /* terminate context at C side 
                 * Contexts can be removed by shutdown hooks...
                 */
                if (ctxMap.get((Long)ctx) != 0)
                {
                   tplogError(">>> About to GC: %x!!!", ctx);
                   finalizeC(ctx);
                   ctxMap.remove((Long)ctx);
                }
            }
            finally {
                ctx = 0;
                ctxMapMutex.unlock();
            }
        }
    }
    
    /**
     * Kill all (terminate all contexts)
     */
    static void destructAll()
    {
        ctxMapMutex.lock();
        try
        {
            Iterator it = ctxMap.entrySet().iterator();
            while (it.hasNext()) {
                
                Map.Entry pair = (Map.Entry)it.next();
                Long cPtr = (Long)pair.getKey();
                finalizeC((long)cPtr);
                it.remove();
            }
            
            /* Delete all from hash map here? */
        } finally {
            ctxMapMutex.unlock();
        }
    }
    
    /**
     * Register shutdown hook for cleaning up ATMI context instances
     */
    static{
        try{
            
        Runtime.getRuntime().addShutdownHook(new Thread() {
            public void run() {
                destructAll();
            }
        });
            
        }catch(Exception e){
            throw new RuntimeException("Exception occured in creating singleton instance");
        }
    }
    
    /**
     * Terminate the context at C side (tpterm + remove ctx by it self)
     * @param cPtr ATMI Context pointer
     */
    private static native void finalizeC(long cPtr);
    
    /**
     * Terminate the ATMI Context
     * @throws Throwable 
     */
    @Override
    protected void finalize() throws Throwable {

        cleanup();

        //Remove ATMI context...
        super.finalize();
    }
    
    /**
     * Perform tp return
     * @param rval return value (TPSUCCESS/TPFAIL)
     * @param rcode user return code
     * @param data data buffer to return, can be NULL to?
     * @param flags 
     */
    public native void tpreturn(int rval, long rcode, TypedBuffer data, long flags);
    
    /**
     * Forward the call to other service for processing
     * @param svc target service name
     * @param data data buffer to send
     * @param flags call flags (reserved )
     */
    public native void tpforward(String svcname, TypedBuffer data, long flags);
    
    /**
     * Incoming service call dispatch to advertised service.
     * Called by C side
     * @param svcInfo service call infos
     */
    void tpCallDispatch(TpSvcInfo svcInfo) {
        try
        {
            /* the exception will be captured at C side */
            svcMap.get(svcInfo.getName()).tpService(this, svcInfo);
        }
        catch (Exception e)
        {
            //Return fail with the same buffer in case of exception!
            tpreturn(AtmiConstants.TPFAIL, ctx, svcInfo.data, ctx);
        }
    }

    /**
     * Run server in context
     * @param arg Command line arguments passed to java
     * @param nocheck Do not check the arguments
     */
    private native int tpRunC(String[] arg, boolean nocheck);
    
    /**
     * Run server instance. Only one thread is allowed to step into this
     * @param svr server class
     * @param arg Command line argumenst passed to the Enduro/X core. This is
     *  optional and can be NULL. In that case NDRX_SV* environment variables
     *  are used.
     * @return -1 (failed with out specified error) or 0 Success (finished ok)
     * @throws  AtmiTPEINVALException invalid command line arguments or invalid
     *  ATMI context.
     */
    public synchronized int tprun(Server svr, String[] arg)
    {
        if (null==svr)
        {
            throw new AtmiTPEINVALException("svr argument is null!");
        }
        
        this.svr = svr;
        
        tplogndrx(AtmiConstants.LOG_INFO, "Booting server");
        
        /* Call native server entry (this should in return boot call server
         * interface 
         */
        return tpRunC(arg, false);
    }
    
    /**
     * Run server using Enduro/X environment variables
     * @param svr server class instance
     * @return -1 (failed with out specified error) or 0 Success (finished ok)
     * @throws  AtmiTPEINVALException invalid command line arguments or invalid
     *  ATMI context.
     */
    public synchronized int tprun(Server svr)
    {
        if (null==svr)
        {
            throw new AtmiTPEINVALException("svr argument is null!");
        }
        
        this.svr = svr;
        
        tplogndrx(AtmiConstants.LOG_INFO, "Booting server");
        
        /* Call native server entry (this should in return boot call server
         * interface 
         */
        return tpRunC(null, true);
    }
    
    /**
     * TP logger
     * @param lev log level
     * @param file optional file name
     * @param line optional line number in file (if no metadata infos, use -1)
     * @param message log message
     */
    private native void tplogC(int lev, String file, long line, String message);

    /**
     * NDRX package internal logger
     * @param lev log level
     * @param file optional file name
     * @param line optional line number in file (if no metadata infos, use -1)
     * @param message log message
     */
    private native void tplogndrxC(int lev, String file, long line, String message);
    
    /**
     * Query logger information
     * @param lev current log level
     * @param flags  See TPLOGQI_GET_ and TPLOGQI_EVAL_ flag constants
     * @return LOG_FACILITY_ bits, TPLOGQI_RET_ bits, and bits from 24..32 
     *  represents log level.
     */
    public native int tplogqinfo(int lev, long flags);
    
    /**
     * Call the C side of advertise. This basically performs the low level
     * C side advertise of the internal dispatching method.
     * During the service call dispatching method shall resolve the actual object
     * of the "svc" and invoke tpService().
     * @param svcname Service name
     * @param funcname Function name
     * @throws AtmiTPEOSException System failure occurred during serving. 
     *    See logs i.e. user log, or debugs for more info. That could insufficient 
     *    memory or other error.
     */
    private native void tpadvertiseC(String svcname, String funcname);

    /**
     * Advertise service.
     * @param svcname Service name
     * @param funcname Function name
     * @param svc interface to object implementing Service
     * @throw AtmiTPEOSException System failure occurred during serving. 
     *    See logs i.e. user log, or debugs for more info. That could insufficient 
     *    memory or other error.
     */
    public void tpadvertise(String svcname, String funcname, Service svc) {
    
	/* call the native interface - advertise service*/
        tpadvertiseC(svcname, funcname);
	
	/* add service to hash list */
        svcMap.put(svcname, svc);
    }

    /**
     * Write the ndrx log (for library internal use). Package level access.
     * @param lev Log level
     * @param directCall is this function called directly or via logger wrapper?
     * @param format format string
     * @param arguments  format arguments
     */
    void tplogndrx(int lev, String format, Object... arguments) {

        int log_config = tplogqinfo(lev,
                AtmiConstants.TPLOGQI_GET_NDRX | AtmiConstants.TPLOGQI_EVAL_DETAILED);

        String filename = "";
        long line = AtmiConstants.FAIL;

        if (log_config <= 0) {

            /* nothing to log */
            return;
        }

        if ((log_config & AtmiConstants.TPLOGQI_EVAL_DETAILED) > 0) {

            /* backtrace the file and line number */
            StackTraceElement[] s = Thread.currentThread().getStackTrace();

            filename = s[2].getFileName();
            line = s[2].getLineNumber();
        }
        /* write the log according to the detail level with or with out
         * stack tracking
         */
        tplogndrxC(lev, filename, line, String.format(format, arguments));
    }

    /**
     * Write the user log
     * @param lev Log level
     * @param directCall is this function called directly or via logger wrapper?
     * @param format format string
     * @param arguments  format arguments
     */
    public void tplog(int lev, boolean directCall, String format, Object... arguments) {
        
        int log_config = tplogqinfo(lev, 
                AtmiConstants.TPLOGQI_GET_TP | AtmiConstants.TPLOGQI_EVAL_DETAILED);
        
        String filename = "";
        long line = AtmiConstants.FAIL;
        
        if (log_config <= 0) {
            
            /* nothing to log */
            return;
        }
        
        if ((log_config & AtmiConstants.TPLOGQI_RET_HAVDETAILED) > 0) {
            
            /* backtrace the file and line number */
            StackTraceElement[] s = Thread.currentThread().getStackTrace();
            
            if (directCall) {
                filename = s[2].getFileName();
                line = s[2].getLineNumber();
            } else {
                filename = s[3].getFileName();
                line = s[3].getLineNumber();
            }
        }
        /* write the log according to the detail level with or with out
         * stack tracking
         */
        tplogC(lev, filename, line, String.format(format, arguments));
    }
    
    /**
     * Log Always
     * @param format format string
     * @param arguments variable args 
     */
    public void tplogAlways(String format, Object... arguments) {
        tplog(AtmiConstants.LOG_ALWAYS, false, format, arguments);
    }   
    
    /**
     * Log Error
     * @param format format string
     * @param arguments variable args 
     */
    public void tplogError(String format, Object... arguments) {
        tplog(AtmiConstants.LOG_ERROR, false, format, arguments);
    }
    
    /**
     * Log Warning
     * @param format format string
     * @param arguments variable args 
     */
    public void tplogWarn(String format, Object... arguments) {
        tplog(AtmiConstants.LOG_WARN, false, format, arguments);
    }
    
    /**
     * Log Info
     * @param format format string
     * @param arguments variable args 
     */
    public void tplogInfo(String format, Object... arguments) {
        tplog(AtmiConstants.LOG_INFO, false, format, arguments);
    }
    
    /**
     * Log Debug
     * @param format format string
     * @param arguments variable args 
     */
    public void tplogDebug(String format, Object... arguments) {
        tplog(AtmiConstants.LOG_DEBUG, false, format, arguments);
    }
    
    /**
     * Log Dump
     * @param format format string
     * @param arguments variable args 
     */
    public void tplogDump(String format, Object... arguments) {
        tplog(AtmiConstants.LOG_DUMP, false, format, arguments);
    }
    
    /**
     * Initialize current ATMI Context as a ATMI client
     * @param tpinfo might be NULL. Currently not used by Enduro/X
     * @thorw AtmiTPEINVALException environment not configured
     * @throw AtmiTPESYSTEMException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info.
     * @throw AtmiTPEOSException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info.
     */
    public native void tpinit(TpInit tpinfo);
    
    /**
     * Write user log message
     * @param msg formatted message to log in user log
     */
    private native void userlogC(String msg);
    
    /**
     * Write user log (for more critical events)
     * @param format java format string
     * @param arguments arguments for format string
     */
    public void userlog(String format, Object... arguments) {
        userlogC(String.format(format, arguments));
    }
    
    /**
     * Call the service
     * @param svc XATMI service name to call
     * @param idata Input buffer
     * @param flags ATMI flags; TPNOTRAN, TPSIGRSTRT, TPNOTIME, TPNOCHANGE, TPTRANSUSPEND,
     *  TPNOBLOCK
     * @return We return the buffer form the call.
     */
    public native TypedBuffer tpcall(String svc, TypedBuffer idata, long flags);
    
    
    /**
     * Group of boolean expression routines
     * @defgroup Bbool boolean expression handling routines
     * @{
     */
    
    /**
     * Compile boolean expression
     * @param expr UBF boolean expression
     * @return Compiled boolean expression handler
     */
    public native BExprTree Bboolco(String expr);
    
    /**
     * Print compiled expression to output stream.
     * @param cexpr compiled expression
     * @param ostream output stream. write(byte[] bytes) is used for printing
     *  the output.
     */
    public native void Bboolpr(BExprTree cexpr, OutputStream ostream);
    
    /**
     * Set Expression evaluator function callback.
     * Then C will proxy back to java side for actual function invoke
     * @param funcname function name to register for evaluator
     */
    native void BboolsetcbfC (String funcname);
    
    /**
     * UBF expression callback mappings 
     */
    static Map<String, Bboolcbf> ubfcbMap = new HashMap<String, Bboolcbf>();
    
    /**
     * Lock for hashmap of the UBF callback
     */
    static final Lock ubfcbMapMutex = new ReentrantLock(true);
    
    /**
     * Get the boolean expression callback object in synchronized way
     * @param funcname function for which to lookup
     * @return null if  not found, not null if object found
     */
    static Bboolcbf BoolgetcbfObj(String funcname) {
        
        ubfcbMapMutex.lock();
        
        try
        {
            return ubfcbMap.get(funcname);
        }
        finally
        {
            ubfcbMapMutex.unlock();
        }
    }
    
    /**
     * Set callback function from the expression, so that this function
     * can be referenced/called from the compiled script/evaluated UBF boolean
     * expression script. This function is not thread safe. meaning that
     * any other thread shall not execute Boolean expressions while the
     * callback are being registered.
     * The function registers the callbacks at the java side into hashmap.
     * and registration is done at C side too. The mapping is done in a singleton
     * as it might be used by different contexts.
     * @param funcname function name (C style function name)
     * @param callback interface to callback object
     */
    public void Bboolsetcbf (String funcname, Bboolcbf callback) {
        
        if (null==callback) {
            throw new AtmiTPEINVALException("callback argument must not be null!");
        }
        
        /* register the callback func down to C level */
        BboolsetcbfC(funcname);
                
        /* Register at java level */
        ubfcbMapMutex.lock();
        try
        {
            ubfcbMap.put(funcname, callback);
        }
        finally
        {
            ubfcbMapMutex.unlock();
        }
        
    }
    
    /** @} */ // end of Bbool
    
}

/* vim: set ts=4 sw=4 et smartindent: */
