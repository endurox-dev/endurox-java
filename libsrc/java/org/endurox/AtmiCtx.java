/**
 * @brief Enduro/X ATMI Context Class
 *   This is main class used for almost all operations with Enduro/X middleware
 *
 * @class AtmiCtx
 */
/* -----------------------------------------------------------------------------
 * Enduro/X Middleware Platform for Distributed Transaction Processing
 * Copyright (C) 2009-2016, ATR Baltic, Ltd. All Rights Reserved.
 * Copyright (C) 2017-2019, Mavimax, Ltd. All Rights Reserved.
 * This software is released under one of the following licenses:
 * LGPL or Mavimax's license for commercial use.
 * See LICENSE file for full text.
 *
 * C (as designed by Dennis Ritchie and later authors) language code is licensed
 * under Enduro/X Modified GNU Affero General Public License, version 3.
 * See LICENSE_C file for full text.
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
package org.endurox;

import java.io.OutputStream;
import java.lang.reflect.Method;
import java.sql.Connection;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.Properties;
import org.endurox.exceptions.AtmiTPEINVALException;
import org.endurox.exceptions.AtmiTPESYSTEMException;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;
import javax.sql.XAConnection;
import javax.sql.XADataSource;
import javax.transaction.xa.XAException;
import javax.transaction.xa.XAResource;
import javax.transaction.xa.Xid;
import org.endurox.exceptions.AtmiTPEBADDESCException;
import org.endurox.exceptions.AtmiTPEBLOCKException;
import org.endurox.exceptions.AtmiTPEDIAGNOSTICException;
import org.endurox.exceptions.AtmiTPELIMITException;
import org.endurox.exceptions.AtmiTPENOENTException;
import org.endurox.exceptions.AtmiTPEOSException;
import org.endurox.exceptions.AtmiTPEPROTOException;
import org.endurox.exceptions.AtmiTPESVCERRException;
import org.endurox.exceptions.AtmiTPESVCFAILException;
import org.endurox.exceptions.AtmiTPETIMEException;
import org.endurox.exceptions.UbfBALIGNERRException;
import org.endurox.exceptions.UbfBBADFLDException;
import org.endurox.exceptions.UbfBBADNAMEException;
import org.endurox.exceptions.UbfBNOTFLDException;
import org.endurox.exceptions.UbfBNOTPRESException;
import org.endurox.exceptions.UbfBSYNTAXException;
import org.endurox.exceptions.UbfBTYPERRException;

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
 * @subsection errorhandling_sec Error handling
 * In Enduro/X Java module error handling is done in a Java native way exceptions
 * are thrown. Non checked exceptions are used and exceptions which might be
 * throw by underlaying Enduro/X C API calls are denoted in documentation.
 * There might be other exceptions thrown by the APIs, in case if there are
 * some issues with JNI calls, like out of memory or missing classes. This
 * normally shall not happen.
 * 
 * ATMI Exceptions may contain the input buffer assocated with the exception.
 * If programmer have passed to method TypedBuffer and the exception does
 * not contain the buffer, then assume that input buffer is still valid.
 * If exception contains the buffer, then programmer shall continue to use that
 * one from exception.
 */
public class AtmiCtx {
	
    /**
     * Pointer to C ATMI Context object
     */
    long ctx = 0;
    
    /**
     * XA Data source for distributed transaction processing
     * This is singleton shared between threads. Initialized
     * when Enduro/X Boots.
     */
    static XADataSource xads = null;
    
    /**
     * XA Connection handler, i.e. if the context have invoked
     * tpopen().
     */
    public XAConnection xaConn = null;
    
    /**
     * Currently active connection
     
    public Connection dbConn = null;
    */
    
    /**
     * Currently active resource
     */
    public XAResource xaRes = null;

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
        /* Use external loader??? */
       System.loadLibrary("exjavald"); // Load native library at runtime
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
     * Unsolicited callback handler
     */
    UnsolCallback unslcb = null;
    
    
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
        /* This throws TPESYSTEM if failed.*/
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
     * Create new ATMI Context, from existing C/external context.
     * This does not perform C context deallocation
     * @param ctx ptr to C context
     */
    public AtmiCtx(long ctx) {        
        this.ctx = ctx;
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
                   tplogInfo(">>> About to GC: %x!!!", ctx);
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
            //Log exception (i.e. backtrace)...
            
            tplogndrxex(AtmiConst.LOG_ERROR, 
                    String.format("Service [%s] generated exception", 
                            svcInfo.getName()), e);
            
            //Return fail with the same buffer in case of exception!
            tpreturn(AtmiConst.TPFAIL, AtmiConst.TPESVCERR, 
                    svcInfo.data, AtmiConst.TPSOFTERR);
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
        
        tplogndrx(AtmiConst.LOG_INFO, "Booting server");
        
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
        
        tplogndrx(AtmiConst.LOG_INFO, "Booting server");
        
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
     * List of text logging functions for log file processing.
     * @defgroup Logging functions
     * @{
     */
    
    /**
     * Delete request file name from buffer
     * @param data typed buffer 
     * @throws AtmiTPENOENTException Filed not present or failed to remove from UBF.
     * @throws AtmiTPEINVALException Not UBF buffer or buffer NULL.
     * @throws NullPointerException data is NULL.
     */
    public native void tplogdelbufreqfile(TypedBuffer data);

    /**
     * Query logger information
     * @param lev current log level
     * @param flags  See TPLOGQI_GET_ and TPLOGQI_EVAL_ flag constants
     * @return LOG_FACILITY_ bits, TPLOGQI_RET_ bits, and bits from 24..32 
     *  represents log level.
     */
    public native int tplogqinfo(int lev, long flags);
    
    /**
     * Log exception to UBF logger
     * @param lev debug level
     * @param msg custom message
     * @param e exception to backtrace
     */
    native void tplogubfex(int lev, String msg, Throwable e);
    
    /**
     * Log exception to NDRX logger
     * @param lev debug level
     * @param msg custom message
     * @param e exception to back
     */
    native void tplogndrxex(int lev, String msg, Throwable e);
    
    /**
     * Log exception to user logger
     * @param lev debug level
     * @param msg trace message
     * @param e exception to backtrace
     */
    public native void tplogex(int lev, String msg, Throwable e);
    
    /**
     * Set request log file name. This switches the current logger for tp, ndrx and ubf
     * to specific file name. The logfile name can be present in typed buffer
     * (i.e. UBF buffer field EX_NREQLOGFILE) - parameter \data, or file name 
     * can be specified in \p filename argument if these parameters are not
     * set or file name os not there, function will attempt to call \p filesvc
     * service (if present). In case of if function is unable to locate the
     * filename, the exception is thrown.
     * 
     * If UBF buffer is passed and file name is only present in \p filename,
     * then function sill set field EX_NREQLOGFILE to UBF \p data buffer.
     * 
     * See tplogsetreqfile(3) manpage for more information.
     * 
     * @param data Typed Buffer (supports only UBF buffer currently) with EX_NREQLOGFILE
     *  field present. This buffer also is used for doing \p filesvc calls. In
     *  case if file name is available in \p filename and buffer data is present,
     *  the value is copied to EX_NREQLOGFILE. It is assumed that buffer has
     *  enough space to set the EX_NREQLOGFILE field. Parameter is optional
     *  and might be passed as null.
     * 
     * @param filename File name to which set the request logging.
     * 
     * @param filesvc fallback service name to request for log file name (EX_NREQLOGFILE)
     *  in case if \p data and \p filename does not contain the file name to
     *  switch logging to.
     * 
     * @throws AtmiTPEINVALException Missing file name in parameters 
     *  (invalid parameter combination).
     * @throws AtmiTPENOENTException No service (filesvc parameter) 
     *  advertised in system.
     * @throws AtmiTPETIMEException Service did not reply in given time 
     *  (NDRX_TOUT).
     * @throws AtmiTPESVCFAILException Service returned TPFAIL. 
     *  This is application level failure.
     * @throws AtmiTPESVCERRException System level service failure. Server 
     *  died during the message presence in service queue.
     * @throws AtmiTPESYSTEMException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info.
     * @throws AtmiTPEOSException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info.
     */
    public native void tplogsetreqfile(TypedBuffer data, String filename, String filesvc);
    
    /**
     * Read the request file name from the given \p data buffer. Currently
     * only UBF buffer is supported and field name must be EX_NREQLOGFILE.
     * 
     * See tploggetbufreqfile(3) manpage for more information.
     * 
     * @param data Typed buffer, UBF supported only.
     * @return current logging file name.
     * @throws AtmiTPENOENTException Request logging file name not present 
     *  in UBF buffer or system failure in reading from UBF.
     * @throws AtmiTPEINVALException Not UBF buffer.
     * 
     * @throws NullPointerException data is NULL.
     */
    public native String tploggetbufreqfile(TypedBuffer data);
    
    /**
     * Get current request logging file.
     * In case if request logging is not used, method returns null value.
     * 
     * See tploggetreqfile(3) manpage for more information.
     * 
     * @return current request logging file. In case if request logging is
     *  not used, null value is returned.
     */
    public native String tploggetreqfile();
    
    /**
     * Change logger file directly from the given file name. If there is problem
     * with setting the output log file, the logger will be changed to stderr.
     * 
     * See tplogsetreqfile_direct(3) manpage for more information.
     * 
     * @param filename file name to which set the request logger
     * 
     * @throws NullPointerException In case if \p filename is null.
     */
    public native void tplogsetreqfile_direct(String filename);
    
    /**
     * Close request file.
     * The logger will return to thread logger (if was configured) or process logger.
     * 
     * See tplogclosereqfile(3) manpage for more information.
     */
    public native void tplogclosereqfile();
    
    /**
     * Configure logger.
     * Function does configure logging facilities - NDRX (XATMI internal logs), 
     * UBF (UBF internal logs( and TP (User logs). If not already logger started, 
     * then this method will initiate Enduro/X framework to load the logging
     * settings from [@debug] ini section or from ndrxdebug.conf(5). 
     * Then with help of this method user is able to override loaded settings.
     * 
     * Also it is possible to set per thread logging, if facility code used 
     * here is LOG_FACILITY_TP_THREAD. Or it is possible to configure request 
     * based logging from thus method, but tplogsetreqfile_direct(3) and
     * tplogsetreqfile(3) is recommended to use instead.
     * 
     * See tplogconfig(3) manpage for more information.
     * 
     * @param logger see AtmiConst.LOG_FACILITY... constants, can be binary OR'd
     * @param lev log level, see AtmiConst.LOG_DUMP,..,AtmiConst.LOG_ALWAYS
     * @param debug_string debug string according to ndrxdebug.conf(5)
     * @param module module name which does the logging (is used in log file line, 4 symbols)
     * @param new_file New log file to use for logging
     * 
     * @throws AtmiNEFORMATException Invalid format for debug_string.
     * @throws AtmiNESYSTEMException System error occurred. 
     *  See the logs for more info.
     */
    public native void tplogconfig(int logger, int lev, String debug_string, String module, String new_file);
    
    /**
     * Log exception to ulog
     * @param msg custom message
     * @param e exception to backtrace
     */
    public native void userlogex(String msg, Throwable e);
    
    /**
     * Write the ndrx log (for library internal use). Package level access.
     * @param lev Log level
     * @param directCall is this function called directly or via logger wrapper?
     * @param format format string
     * @param arguments  format arguments
     */
    void tplogndrx(int lev, String format, Object... arguments) {

        int log_config = tplogqinfo(lev,
                AtmiConst.TPLOGQI_GET_NDRX | AtmiConst.TPLOGQI_EVAL_DETAILED);

        String filename = "";
        long line = AtmiConst.FAIL;

        if (log_config <= 0) {

            /* nothing to log */
            return;
        }

        if ((log_config & AtmiConst.TPLOGQI_EVAL_DETAILED) > 0) {

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
                AtmiConst.TPLOGQI_GET_TP | AtmiConst.TPLOGQI_EVAL_DETAILED);
        
        String filename = "";
        long line = AtmiConst.FAIL;
        
        if (log_config <= 0) {
            
            /* nothing to log */
            return;
        }
        
        if ((log_config & AtmiConst.TPLOGQI_RET_HAVDETAILED) > 0) {
            
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
        tplog(AtmiConst.LOG_ALWAYS, false, format, arguments);
    }   
    
    /**
     * Log Error
     * @param format format string
     * @param arguments variable args 
     */
    public void tplogError(String format, Object... arguments) {
        tplog(AtmiConst.LOG_ERROR, false, format, arguments);
    }
    
    /**
     * Log Warning
     * @param format format string
     * @param arguments variable args 
     */
    public void tplogWarn(String format, Object... arguments) {
        tplog(AtmiConst.LOG_WARN, false, format, arguments);
    }
    
    /**
     * Log Info
     * @param format format string
     * @param arguments variable args 
     */
    public void tplogInfo(String format, Object... arguments) {
        tplog(AtmiConst.LOG_INFO, false, format, arguments);
    }
    
    /**
     * Log Debug
     * @param format format string
     * @param arguments variable args 
     */
    public void tplogDebug(String format, Object... arguments) {
        tplog(AtmiConst.LOG_DEBUG, false, format, arguments);
    }
    
    /**
     * Write the hex dump to the log file
     * @param msg Message to print in log
     * @param data bytes to dump off
     */
    public native void tplogDump(int lev, String msg, byte[] data);
    
    /**
     * Print the differences between two binary buffers
     * @param lev log level at which to print
     * @param msg hex dump message
     * @param data1 first byte block
     * @param data2 second byte block to compare
     */
    public native void tplogDumpDiff(int lev, String msg, byte[] data1, byte[] data2);
    
    
    /** @} */ // end of Logging
    
    /**
     * Initialize current ATMI Context as a ATMI client
     * @param tpinfo might be NULL. Currently not used by Enduro/X
     * @throws AtmiTPEINVALException environment not configured
     * @throws AtmiTPESYSTEMException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info.
     * @throws AtmiTPEOSException System failure occurred during serving. 
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
     * See tpcall(3) manpage for more information.
     * @param svc XATMI service name to call
     * @param idata Input buffer
     * @param flags ATMI flags; TPNOTRAN, TPSIGRSTRT, TPNOTIME, TPNOCHANGE, TPTRANSUSPEND,
     *  TPNOBLOCK
     * @throws AtmiTPEINVALException Invalid parameter is given to function. 
     *  Either service name is NULL or flags does not allow to change the value.
     * @throws AtmiTPENOENTException No service (svc parameter) 
     *  advertised in system.
     * @throws AtmiTPETIMEException Service did not reply in given time 
     *  (NDRX_TOUT).
     * @throws AtmiTPESVCFAILException Service returned TPFAIL. This is 
     *  application level failure.
     * @throws AtmiTPESVCERRException System level service failure. Server 
     *  died during the message presence in service queue.
     * @throws AtmiTPESYSTEMException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info.
     * @throws AtmiTPEOSException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info.
     * @throws AtmiTPEBLOCKException Service request queue was full and 
     *  TPNOBLOCK flag was specified.
     * @throws AtmiTPNOABORTException Do not abort global transaction (if one 
     *  in progress), even if service failed.
     * @return We return the buffer form the call.
     */
    public native TypedBuffer tpcall(String svc, TypedBuffer idata, long flags);
    
    /**
     * Asynchronous service call
     * See tpacall(3) manpage for more information.
     * @param svc service name to call
     * @param idata input typed buffer to send to service
     * @param flags call flags: TPNOTRAN, TPSIGRSTRT, TPNOBLOCK, TPNOREPLY
     * @throws AtmiTPEINVALException Invalid parameter is given to function. 
     *  Either service name is NULL or flags does not allow to change the value.
     * @throws AtmiTPENOENTException No service (svc parameter) advertised in system.
     * @throws AtmiTPETIMEException Service did not reply in given time (NDRX_TOUT).
     * @throws AtmiTPESVCFAILException Service returned TPFAIL. This is 
     *  application level failure.
     * @throws AtmiTPESVCERRException System level service failure. Server 
     *  died during the message presence in service queue.
     * @throws AtmiTPESYSTEMException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info.
     * @throws AtmiTPEOSException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info.
     * @throws AtmiTPEBLOCKException Service queue was full and 
     *  TPNOBLOCK flag was specified.
     * @throws AtmiTPNOTIMEException Do not expire call by server process, 
     *  If message age is older than NDRX_TOUT timeout (or timeout 
     *  overridden by tptoutset(3)).
     * @return call descriptor
     */
    public native int tpacall(String svc, TypedBuffer idata, long flags);
    
    /**
     * Get reply from Asynchronous call tpacall().
     * See tpgetrply(3) manpage for more information.
     * @param[in] cd call descriptor returned by  \ref tpacall(). In case if 
     *  TPGETANY flag is set, the field value is ignored
     * @param[in] odata input data buffer used for receiving the reply data
     *  copied/reallocated to TpgetrplyResult
     * @param flags  TPGETANY, TPNOBLOCK, TPSIGRSTRT, TPNOTIME,
     *  TPNOCHANGE, TPNOABORT 
     * @return result buffer and call descriptor of call returned.
     * @throws AtmiTPEBLOCKException *TPENOBLOCK* was specified in flags and no 
     *  message is in queue.
     * @throws AtmiTPEINVALException Invalid parameter is given to function. 
     *  Particularly pointer parameters are NULL.
     * @throws AtmiTPETIMEException Service did not reply in 
     *  given time (NDRX_TOUT time).
     * @throws AtmiTPESVCFAILException Service returned TPFAIL. 
     *  This is application level failure.
     * @throws AtmiTPESVCERRException System level service failure. 
     *  Server died during the message presence in service queue.
     * @throws AtmiTPESYSTEMException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info.
     * @throws AtmiTPEOSException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info.
     * @throws AtmiTPEBADDESCException Invalid call descriptor - out of range or
     *  not issued/canceled value. This exception is thrown only in case if TPGETANY
     *  flag is not passed to the call.
     */
    public native TpgetrplyResult tpgetrply(int cd, TypedBuffer odata, long flags);
    
    /**
     * Conversational methods group
     * @defgroup Convers conversational methods
     * @{
     */
    
    /**
     * Connect to XATMI conversational server
     * See tpconnect(3) manpage for more information.
     * @param svc service name
     * @param idata input XATMI buffer
     * @param flags AtmiConst flags - TPNOTRAN, TPSIGRSTRT, TPNOTIME, TPTRANSUSPEND,
     *  TPSENDONLY, TPRECVONLY. Binary or'able.
     * @return connection descriptor (cd)
     * @throws AtmiTPEINVALException Invalid parameter is given to function. 
     *  Either service name is NULL or data is not NULL, but not allocated by tpalloc()
     * @throws AtmiTPENOENTException No service (svc parameter) 
     *  advertised in system.
     * @throws AtmiTPELIMITException Max number of connections are reached.
     *  Currently max number of connections is limited to 5 (MAX_CONNECTIONS macro).
     * @throws AtmiTPETIMEException Service did not reply in 
     *  given time (NDRX_TOUT).
     * @throws AtmiTPESVCFAILException Service returned TPFAIL. This is 
     *  application level failure.
     * @throws AtmiTPESVCERRException System level service failure. Server 
     *  died during the message presence in service queue.
     * @throws AtmiTPESYSTEMException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info.
     * @throws AtmiTPEOSException System failure occurred during serving. See 
     *  logs i.e. user log, or debugs for more info.
     */
    public native int tpconnect(String svc, TypedBuffer idata, long flags);
    
    /**
     * Send data to conversational endpoint
     * See tpsend(3) manpage for more information.
     * @param cd conversation descriptor
     * @param idata input data buffer. Data buffer is valid after the call
     * @param flags TPRECVONLY, TPNOBLOCK, TPSIGRSTRT
     * @return receive event. If sent ok w/o event, then return code is 0.
     *  Event constants: TPEV_SVCERR, TPEV_SVCFAIL
     * @throws AtmiTPEINVALException Invalid call descriptor cd passed in or 
     *  data pointer is not pointing to buffer allocated by tpalloc().
     * @throws AtmiTPETIMEException Was unable to send message in given 
     *  time(NDRX_TOUT env param.).
     * @throws AtmiTPESYSTEMException System failure occurred during 
     *  serving. See logs i.e. user log, or debugs for more info.
     * @throws AtmiTPEOSException System failure occurred during 
     *  serving. See logs i.e. user log, or debugs for more info.
     */
    public native long tpsend(int cd, TypedBuffer idata, long flags);
    
    /**
     * Received data from endpoint
     * @param cd conversation id
     * @param idata input data buffer into which received data shall be stored.
     *  This object becomes invalid after the call, new instance is provided
     *  in return object. The data type may be changed of the buffer.
     * @param flags TPRECVONLY, TPNOBLOCK, TPSIGRSTRT, TPNOBLOCK
     * @return Receive result (event, cd, typed buffer). Possible event constants:
     *  TPEV_DISCONIMM, TPEV_SENDONLY, TPEV_SVCERR, TPEV_SVCFAIL, TPEV_SVCSUCC
     * 
     * @throws AtmiTPEINVALException Invalid call descriptor cd passed in.
     * @throws AtmiTPETIMEException Service did not reply in given time (NDRX_TOUT).
     * @throws AtmiTPEPROTOException System level service failure. Server died during 
     *  the message presence in service queue.
     * @throws AtmiTPESYSTEMException System failure occurred during serving. See logs 
     *  i.e. user log, or debugs for more info.
     * @throws AtmiTPEOSException System failure occurred during serving. See logs i.e. 
     *  user log, or debugs for more info.
     */
    public native TprecvResult tprecv(int cd, TypedBuffer idata, long flags);
    
    /**
     * Disconnect from conversation
     * @param cd conversation descriptor / id
     * @throws AtmiTPEINVALException Invalid connection descriptor passed in.
     * @throws AtmiTPEOSException System failure occurred during serving. See 
     * logs i.e. user log, or debugs for more info.
     */
    public native void tpdiscon(int cd);
    
    /** @} */ // end of Convers
    
    /**
     * Cancel a tpacall(). This basically marks the in the call descriptor registry
     * that response is no more waited from the caller.
     * @param cd call descriptor from the tpacall()
     * @throws AtmiTPEBADDESCException Invalid call descriptor (out of range).
     * @throws AtmiTPEINVALException Enduro/X is not configured.
     * @throws AtmiTPESYSTEMException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info.
     * @throws AtmiTPEOSException System failure occurred during serving. 
     * See logs i.e. user log, or debugs for more info.
     */
    public native void tpcancel(int cd);
    
    /**
     * tpsetunsol(3) tpbroadcast(3) tpchkunsol(3) tpnotify(3)
     * tpdequeue(3) tpdequeueex(3) tpenqueue tpenqueueex
     * tpabort(3) tpbegin(3) tpcommit(3) tpopen(3) tpclose(3) tpsuspend(3) tpresume(3)
     */
    
    /**
     * Group of boolean expression routines
     * @defgroup Bbool boolean expression handling routines
     * @{
     */
    
    /**
     * Compile boolean expression.
     * For more information see Bboolco(3) manpage
     * @param expr UBF boolean expression
     * @return Compiled boolean expression handler
     * @throws UbfBALIGNERRException Corrupted buffer or pointing to not aligned memory area.
     * @throws UbfBNOTFLDException Buffer not fielded, not correctly allocated or corrupted.
     * @throws UbfBBADFLDException Invalid field id passed.
     * @throws UbfBNOTPRESException Field not present.
     * @throws UbfFMALLOCException Malloc failed.
     * @throws UbfBSYNTAXException Synax error in script.
     * @throws UbfBBADNAMEException Bad field name specified.
     */
    public native BExprTree Bboolco(String expr);
    
    /**
     * Print compiled expression to output stream.
     * For more information see Bboolpr(3) manpage
     * @param cexpr compiled expression
     * @param ostream output stream. write(byte[] bytes) is used for printing
     *  the output.
     */
    public native void Bboolpr(BExprTree cexpr, OutputStream ostream);
    
    /**
     * Set Expression evaluator function callback.
     * Then C will proxy back to java side for actual function invoke.
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
     * For more information see Bboolsetcbf(3) manpage.
     * @param funcname function name (C style function name)
     * @param callback interface to callback object
     * @throws UbfBBADNAMEException Invalid function name. See Bboolsetcbf(3)
     *  manpage.
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
    
    
    /**
     * Return field type in string format. The possible values are following:
     * 'short', 'long', 'char', 'float', 'double', 'string', 'carray'
     * See Btype(3) manpage for more information.
     * @param bfldid compiled field id.
     * @return returns field type string
     * @throws UbfBTYPERRException Invalid field - 
     *  bad type extracted from oldest bits.
     */
    public native String Btype(int bfldid);
    
    /**
     * Get field name by give compiled field id. If field tables are bad
     * or field is not found, then output id is formatted as: "((BFLDID32)%d)"
     * where "%d" is filled with \p bfldid
     * See Bfname(3) manpage for more information.
     * @param bfldid compiled filed
     * @return String name of the field
     */
    public native String Bfname(int bfldid);
    
    
    /**
     * Method is used to dynamically resolve field id from given field name
     * See Bfldid(3) manpage for more information.
     * @param fldnm Field name
     * @return compiled field id
     * @throws UbfBEINVALException null value passed in \p fldnm
     */
    public native int Bfldid(String fldnm);
    
    /**
     * Return current XATMI timeout for IPC communications.
     * By default method uses NDRX_TOUT env variable for readings if value
     * is not override by \see AtmiCtx::tptoutset
     * See tptoutget(3) manpage for more information.
     * @return timeout in seconds currently used by XATMI IPC sub-system
     */
    public native int tptoutget();
    
    /**
     * Returns last user return code from tpcall/tpgetrply result.
     * See tpurcode(3) manpage for more information.
     * @return User return code (a second argument used to pass in tpreturn)
     */
    public native long tpurcode();
    
    /**
     * Set XATMI sub-system timeout value
     * @param tout number of seconds for XATMI calls like tpcall or tpgetrply
     *  before returning timeout exception
     * See tptoutset(3) manpage for more information.
     * @throws AtmiTPEINVALException tout parameter is less or equal to 0.
     */
    public native void tptoutset(int tout);
    
    /**
     * Allocate string object with value
     * @param s string value to set
     * @return Typed string filled with value
     */
    public TypedString newString(String s) {
        
        TypedString ret = (TypedString)tpalloc("STRING", "", s.getBytes().length);
        
        ret.setString(s);
        
        return ret;
    }
    
    /**
     * Allocate JSON object
     * @param j string value to set
     * @return Typed string filled with value
     */
    public TypedJson newJson(String j) {
        
        TypedJson ret = (TypedJson)tpalloc("JSON", "", j.getBytes().length);
        
        ret.setJSON(j);
        
        return ret;
    }
    
    public TypedCarray newCarray(byte  [] b) {
        
        TypedCarray ret = (TypedCarray)tpalloc("CARRAY", "",b.length);
        
        ret.setBytes(b);
        
        return ret;
        
    }
    
    /**
     * List XATMI buffers (C side pointers)
     * @return List of allocated XATMI buffers.
     *  May be used for debug purposes, i.e. detect gc operations.
     */
    public native long [] getBuffers();
    
    
    /**
     * Unsolicited message handling
     * @defgroup Unsol unsolicited message handling group
     * @{
     */
    
    /**
     * Set the notification handler at C side
     * @param cb null or callback object. Used to detect when to deactivate
     *  the unsolicited message handling.
     */
    native void tpsetunsolC(UnsolCallback cb);
    
    /**
     * Unsolicited message handling
     * Call the "unslcb" callback if have one registered.
     * @param idata input data buffer for callback
     * @param flags flags for callback
     */
    void unsolDispatch(TypedBuffer idata, long flags) {
        
        if (null!=unslcb) {
            unslcb.unsolCallback(this, idata, flags);
        }
    }
    
    /**
     * Register unsolicited message callback handler
     * See tpsetunsol(3) manpage for more information.
     * @param cb callback handler
     * @return previous callback handler
     * @throws AtmiTPEINVALException Environment variables not configured, 
     *  see ex_env(5) page.
     * @throws AtmiTPESYSTEMException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info.
     * @throws AtmiTPEOSException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info.
     */
    public UnsolCallback tpsetunsol(UnsolCallback cb) {
        
        UnsolCallback tmp = this.unslcb;
        
        this.unslcb = cb;
        
        tpsetunsolC(cb);
        
        return tmp;
    }
    
    /**
     * Send notification to clients
     * See tpnotify(3) manpage for more information.
     * @param clientid client id (received in service call)
     * @param idata input typed buffer
     * @param flags valid flags:
     *  - TPNOBLOCK Do not block on full client queue, instead return error.
     *  - TPNOTIME Do not timeout when waiting on full queue (TPNOBLOCK is not set).
     *  - TPSIGRSTRT Restart the system call in progress if interrupted by signal 
     *  handler. This affects only underlaying mq_* function calls.
     *  - TPACK Reserved for future use, Enduro/X silently ignores this flag. 
     *  Thus tpnotify() call does not get any acknowledgement signal that client 
     *  is processed the message. This is limitation of Enduro/X.
     * @throws AtmiTPEINVALException Environment variables not configured, 
     *  see ex_env(5) page, or invalid parameters have been passed to the 
     *  function, for example clientid is NULL or corrupted.
     * @throws AtmiTPENOENTException The local delivery was about to be 
     *  performed (no remote client call) and the client process did not 
     *  exist on local machine. This error will be reported 
     *  regardless of the TPACK flag.
     * @throws AtmiTPETIMEException Blocking message delivery did timeout. 
     *  Meaning that client queue was full and TPNOBLOCK nor TPNOTIME was set. 
     *  Error is returned from local clients only regardless of the TPACK flag. 
     *  If client resists on remote node,
     *  then this error can be returned only when time-out occurred while s
     *  ending message to then local bridge server.
     * @throws AtmiTPEBLOCKException Client queue was full and TPNOBLOCK flag 
     *  was not specified.
     * @throws AtmiTPESYSTEMException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info.
     * @throws AtmiTPEOSException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info.
     */
    public native void tpnotify(ClientId clientid, TypedBuffer idata, long flags);
    
    /**
     * Broadcast a message to matched clients or send message to remove
     * remote server's '@TPBROADNNN' service for local broadcasting.
     * See tpbroadcast(3) manpage for more information.
     * @param lmid cluster node id to which message shall be delivered.
     *  if flag TPREGEXMATCH is present, then regexp is used for the given
     *  string to match the cluster nodes to which message shall be delivered.
     *  Max string length is MAXTIDENT*2-1.
     * @param usrname this is reserved for future use.
     * @param cltname executable name which shall be matched for notification
     *  delivery. If flag TPREGEXMATCH is set, then this is regular expression
     *  of the binary name to be matched. Max string length in bytes
     *  are MAXTIDENT*2-1.
     * @param idata input typed buffer (can be null) for notification delivery
     * @param flags following broadcast flags may be set:
     *  - TPNOBLOCK Do not block on full client queue, instead return error.
     *  - TPNOTIME Do not timeout when waiting on full queue (TPNOBLOCK is not set).
     *  - TPSIGRSTRT Restart the system call in progress if interrupted by 
     *      signal handler. This affects only underlaying mq_* function calls.
     *  - TPREGEXMATCH Match lmid (cluster node id) and cltname by 
     *      assuming that these are regular expressions.
     * @throws AtmiTPEINVALException Environment variables not configured, see 
     *      ex_env(5) page, or invalid parameters have been passed to the function, 
     *      for example clientid, lmtid or username are set and they are invalid 
     *      regular expressions (i.e. with TPREGEXMATCH set).
     * @throws  AtmiTPESYSTEException System failure occurred during serving. 
     *      See logs i.e. user log, or debugs for more info.
     * @throws  AtmiTPEOSException System failure occurred during serving. 
     *      See logs i.e. user log, or debugs for more info.
     */
    public native void tpbroadcast(String lmid, String usrname, String cltname, 
            TypedBuffer idata, long flags);
   
    /**
     * Process received unsolicited messages and invoke callback set by
     *  tpsetunsol(). 
     * See tpchkunsol(3) manpage for more information.
     * @return Number of unsolicited messages processed
     * @throws AtmiTPESYSTEMException System failure occurred during serving. 
     *      See logs i.e. user log, or debugs for more info.
     * @throws AtmiTPEOSException System failure occurred during serving. 
     *      See logs i.e. user log, or debugs for more info.
     */
    public native int tpchkunsol();
    
    /** @} */ // end of Unsol
    
    
    
    /**
     * Queue handling
     * @defgroup Queue Enduro/X queue routines
     * @{
     */
    
    /**
     * Enqueue message to persistent storage
     * See tpenqueue(3) manpage for more information.
     * @param qspace queue space name
     * @param qname queue name
     * @param ctl queue control obj
     * @param idata data buffer
     * @param flags flags TPNOTRAN, TPSIGRSTRT, TPNOTIME, TPNOBLOCK, TPNOABORT
     * @throws  AtmiTPEINVALException data is NULL, qspace is NULL, or 
     *  nodeid and srvid is 0. Error can be generate in case if qname is 
     *  empty or NULL. ctl is NULL or data does not point to 	tpalloc() allocated buffer.
     * @throws  AtmiTPENOENTException Tmqueue server is not available.
     * @throws  AtmiTPETIMEException Service did not reply in given 
     *  time (NDRX_TOUT).
     * @throws  AtmiTPEDIAGNOSTICException More information is provided 
     *  in TPQCTL.diagnostic field. For this exception use getQctl() to get
     *  original TPQCTL passed to method call.
     * @throws  AtmiTPESVCFAILException Tmqueue Service returned TPFAIL. This 
     *  is application level failure.
     * @throws  AtmiTPESVCERRException Tmqueue service got system level failure. 
     *  Server died during the message presence in service queue.
     * @throws  AtmiTPESYSTEMException Enduro/X internal error occurred. 
     *  See logs for more info.
     * @throws  AtmiTPEOSException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info.
     */
    public native void tpenqueue (String qspace, String qname, TPQCTL ctl, 
            TypedBuffer idata, long flags);
    
    /**
     * Enqueue message to persistent storage, extended
     * See tpenqueueex(3) manpage for more information.
     * @param nodeid Cluster node ID on which queue server is present
     * @param srvid Queue Server ID on given cluster node id
     * @param qname queue name
     * @param ctl queue control obj
     * @param idata data buffer
     * @param flags flags TPNOTRAN, TPSIGRSTRT, TPNOTIME, TPNOBLOCK, TPNOABORT
     * @throws  AtmiTPEINVALException data is NULL, qspace is NULL, or 
     *  nodeid and srvid is 0. Error can be generate in case if qname is 
     *  empty or NULL. ctl is NULL or data does not point to 	tpalloc() allocated buffer.
     * @throws  AtmiTPENOENTException Tmqueue server is not available.
     * @throws  AtmiTPETIMEException Service did not reply in given 
     *  time (NDRX_TOUT).
     * @throws  AtmiTPEDIAGNOSTICException More information is provided 
     *  in TPQCTL.diagnostic field. For this exception use getQctl() to get
     *  original TPQCTL passed to method call.
     * @throws  AtmiTPESVCFAILException Tmqueue Service returned TPFAIL. This 
     *  is application level failure.
     * @throws  AtmiTPESVCERRException Tmqueue service got system level failure. 
     *  Server died during the message presence in service queue.
     * @throws  AtmiTPESYSTEMException Enduro/X internal error occurred. 
     *  See logs for more info.
     * @throws  AtmiTPEOSException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info.
     */
    public native void tpenqueueex (short nodeid, short srvid, String qname, TPQCTL ctl, 
            TypedBuffer idata, long flags);
    
    /**
     * Dequeue message from Queue sub-system
     * See tpdequeue(3) manpage for more information.
     * @param qspace queue space name
     * @param qname queue name
     * @param ctl queue control obj
     * @param odata output data to rewrite received message
     * @param flags  TPNOTRAN, TPSIGRSTRT, TPNOTIME, TPNOCHANGE, TPNOBLOCK 
     * @return Returned buffer from queue. Not that \p ctl will contain metadata
     *  about the message
     * @throws  AtmiTPEINVALException data is NULL, qspace is NULL, or nodeid 
     *  and srvid is 0. Error can be generate in case if qname is empty or 
     *  NULL. ctl is NULL or data does not point to tpalloc() allocated buffer.
     * @throws  AtmiTPENOENTException Tmqueue server is not available.
     * @throws  AtmiTPETIMEException Service did not reply in given time (NDRX_TOUT).
     * @throws  AtmiTPEDIAGNOSTICException More information is provided in 
     *  TPQCTL.diagnostic field.
     * @throws  AtmiTPESVCFAILException Tmqueue Service returned TPFAIL. This 
     *  is application level failure.
     * @throws  AtmiTPESVCERRException Tmqueue service got system level failure. 
     *  Server died during the message presence in service queue.
     * @throws  AtmiTPESYSTEMException Enduro/X internal error occurred. 
     *  See logs for more info.
     * @throws  AtmiTPEOSException System failure occurred during serving. 
     * See logs i.e. user log, or debugs for more info.
     */
    public native TypedBuffer tpdequeue (String qspace, String qname, TPQCTL ctl, 
            TypedBuffer odata, long flags);
    
    /**
     * Dequeue message from Queue sub-system
     * See tpdequeueex(3) manpage for more information.
     * @param nodeid Cluster node ID on which queue server is present
     * @param srvid Queue Server ID on given cluster node id
     * @param qname queue name
     * @param ctl queue control obj
     * @param odata output data to rewrite received message
     * @param flags  TPNOTRAN, TPSIGRSTRT, TPNOTIME, TPNOCHANGE, TPNOBLOCK 
     * @return Returned buffer from queue. Not that \p ctl will contain metadata
     *  about the message
     * @throws  AtmiTPEINVALException data is NULL, qspace is NULL, or nodeid 
     *  and srvid is 0. Error can be generate in case if qname is empty or 
     *  NULL. ctl is NULL or data does not point to tpalloc() allocated buffer.
     * @throws  AtmiTPENOENTException Tmqueue server is not available.
     * @throws  AtmiTPETIMEException Service did not reply in given time (NDRX_TOUT).
     * @throws  AtmiTPEDIAGNOSTICException More information is provided in 
     *  TPQCTL.diagnostic field.
     * @throws  AtmiTPESVCFAILException Tmqueue Service returned TPFAIL. This 
     *  is application level failure.
     * @throws  AtmiTPESVCERRException Tmqueue service got system level failure. 
     *  Server died during the message presence in service queue.
     * @throws  AtmiTPESYSTEMException Enduro/X internal error occurred. 
     *  See logs for more info.
     * @throws  AtmiTPEOSException System failure occurred during serving. 
     * See logs i.e. user log, or debugs for more info.
     */
    public native TypedBuffer tpdequeueex (short nodeid, short srvid, String qname, 
            TPQCTL ctl, TypedBuffer odata, long flags);

    /** @} */ // end of Queue
    
    /**
     * Initialize XA Data source
     * Access from package only.
     * This just initalizes data source.
     * tpopen() will acquire connection.
     * @param clazz class to create
     * @param props array of properties to set for data source class
     *  each line consists of key<FS>value
     * @param sets  setting to set for data source class
     *  each line consists of key (func name)<FS>value
     * @return TPERROR Code, so that it can be used from C side.
     */
    int ndrxj_xa_init(String clazz, String []sets) {
        
        Class cls = null;
        
        tplogInfo("Initalizing XADataSource: [%s]", clazz);
        
        try {
            cls = Class.forName(clazz);
        }
        catch (ClassNotFoundException ex) {
            
            tplogex(AtmiConst.LOG_ERROR, String.format("XA Datsource class not found [%s]", 
                    clazz), ex);
            
            return AtmiConst.TPEINVAL; /* <<<< Return! */
        }
        
        try {
            xads = (XADataSource) cls.newInstance();
        } 
        catch (Exception ex) {
            
            tplogex(AtmiConst.LOG_ERROR, String.format("Failed to create intance of [%s]", 
                    clazz), ex);
            return AtmiConst.TPEINVAL; /* <<<< Return! */
        }
        
        
        Method m[] = cls.getMethods();
         
        /* Load the sets... */
        for (int i=0; i<sets.length; i++) {
            
            String []setting = sets[i].split(Character.toString((char)0x1c));
            Properties p = null;
            
            if (setting.length > 2)
            {
                p = new Properties();
                
                tplogDebug("This is property type with %d props for %s", 
                        (setting.length-1) / 2, setting[0]);
                
                /* Load properties values */
                for (int j=1; j<setting.length; j+=2)
                {
                    tplogDebug("Setting param [%s] property [%s] to value [%s]",
                            setting[0], setting[j], setting[j+1]);
                    
                    p.setProperty(setting[j], setting[j+1]);
                }
            }
            else
            {
                tplogDebug("Setting param [%s] to value [%s]",
                            setting[0] , setting[1]);
            }
            
            /* Find the correspoding method and check it's data types.. */
            boolean paramSet = false;
            for(int j = 0; j < m.length; j++) {
                
                if (m[j].getName().equals(setting[0]))
                {
                    tplogDebug("Testing method: %s", m[j].getName());
                    
                    /* Check arguments */
                    
                    /*if (m[j].getParameterCount() !=1)*/
                    if (m[j].getParameterTypes().length !=1)
                    {
                        tplogWarn("Ignoring [%s] with %d argruments, expected 1", 
                                m[j].getName(), m[j].getParameterTypes().length);
                        continue;
                    }
                    
                    /* test first argument type.. */
                    
                    /*String ptype = m[j].getParameters()[0].getType().getName();*/
                    String ptype = m[j].getParameterTypes()[0].getName();
                    try {
                        
                        if (null==p && ptype.equals("java.util.Properties") ||
                                null!=p && !ptype.equals("java.util.Properties")) {
                            tplogError("Invalid parameter for [%s] either it needs properties, "
                                    + "or properties have given to primitive setting [type: %s]", 
                                    setting[0], ptype);
                            return AtmiConst.TPEINVAL;
                        }
                        
                        /* Load short */
                        if (ptype.equals("short") || ptype.equals("java.lang.Short")) {

                            short s = Short.valueOf(setting[1]);
                            m[j].invoke(xads, s);
                            paramSet = true;
                        } 
                        else if (ptype.equals("long") || ptype.equals("java.lang.Long")) {
                            long l = Long.valueOf(setting[1]);
                            m[j].invoke(xads, l);
                            paramSet = true;
                        }
                        else if (ptype.equals("int") || ptype.equals("java.lang.Integer")) {
                            int ii = Integer.valueOf(setting[1]);
                            m[j].invoke(xads, ii);
                            paramSet = true;
                        }
                        else if (ptype.equals("byte") || ptype.equals("java.lang.Byte")) {

                            byte b = Byte.valueOf(setting[1]);
                            m[j].invoke(xads, b);
                            paramSet = true;
                        }
                        else if (ptype.equals("float") || ptype.equals("java.lang.Float")) {
                            float f = Float.valueOf(setting[1]);
                            m[j].invoke(xads, f);
                            paramSet = true;
                        }
                        else if (ptype.equals("double") || ptype.equals("java.lang.Double")) {
                            double d = Double.valueOf(setting[1]);
                            m[j].invoke(xads, d);
                            paramSet = true;
                        }
                        else if (ptype.equals("boolean") || ptype.equals("java.lang.Boolean")) {
                            Boolean b = Boolean.valueOf(setting[1]);
                            m[j].invoke(xads, b);
                            paramSet = true;
                        }
                        else if (ptype.equals("java.lang.String")) {
                            m[j].invoke(xads, setting[1]);
                            paramSet = true;
                        }
                        else if (ptype.equals("java.util.Properties")) {
                            m[j].invoke(xads, p);
                            paramSet = true;
                        }
                        else
                        {
                            tplogError("Type not supported: [%s]", ptype);
                        }
                    } 
                    catch (Exception ex) {
                        tplogex(AtmiConst.LOG_ERROR, String.format("Failed to apply setting [%s]", 
                            setting[0]), ex);
                        return AtmiConst.TPEINVAL; /* <<<< Return! */
                    }
                }
            } /* for methods in class */
            
            if (!paramSet) {
                tplogError("Parameter [%s] not set", setting[0]);
            } else {
                tplogError("Parameter [%s] applied", setting[0]);
            }
            
        } /* for set arguments */        
        
        tplogInfo("XA Data Source [%s] registered OK", clazz);
        
        return AtmiConst.SUCCEED;
    }
    
    /**
     * Check is XA Open
     * @return XA_OK or error
     */
    int xa_is_open() {
        
        if (null==xads) {
            tplogError("xa_is_open: XA Data Source not initialized");
            return AtmiConst.XAER_PROTO;
        }
        
        if (null!=xaConn && null!=xaRes) {
            tplogDebug("xa_is_open: XA is opened");
            return AtmiConst.XA_OK;
        }
        
        tplogDebug("xa_is_open: XA is not opened: dbConn=%s xaRes=%s",
                null!=xaConn?"non NULL":"NULL",
                null!=xaRes?"non NULL":"NULL");
        return AtmiConst.XAER_RMERR;
    }
    
    /**
     * Open XA Entry / Get Connect
     * @param flags not used
     * @return XA_OK, XAER_RMERR
     */
    int xa_open_entry(long flags) {
        
        int ret = AtmiConst.XA_OK;
        
        /* Check some conditions */
        
        if (AtmiConst.XA_OK==xa_is_open())
        {
            tplogError("XA Is already open!");
            return AtmiConst.XAER_PROTO;
        }
        
        try {
            tplogInfo("xa_open_entry: get XA Connection");
            xaConn = xads.getXAConnection();
        }
        catch (SQLException ex) {
            tplogex(AtmiConst.LOG_ERROR, String.format("Failed to get XA "
                    + "connection: SQL state %s", ex.getSQLState()), ex);
            
            return AtmiConst.XAER_RMERR;
        }
        
        try {
            tplogInfo("Getting XA Resource");
            xaRes = xaConn.getXAResource();
        }
        catch (SQLException ex) {
            
            tplogex(AtmiConst.LOG_ERROR, String.format("Failed to get "
                    + "XA resource: SQL state %s", ex.getSQLState()), ex);
            
            xaConn = null;
            return AtmiConst.XAER_RMERR;
        }
        
        return ret;
    }
    
    /**
     * Close entry. Close XA Connections.
     * Cannot be closed if transaction is started?
     * @param flags not used currently
     * @return XA_OK or error code
     */
    int xa_close_entry(long flags) {
        
        /* Close database connection */
        boolean got_err = false;
        
        /* Close XA Connection */
        try {
            if (null!=xaConn) {
                xaConn.close();
                xaConn = null;
            }
        } catch (SQLException ex) {
            tplogex(AtmiConst.LOG_ERROR, String.format("Failed to close xa connection %s",
                    ex.getSQLState()), ex);
            got_err = true;
        }

        /* Forget the XA Resource */
        xaRes = null;
        
        
        if (got_err) {
            return AtmiConst.XAER_RMERR;
        }
        else {
            return AtmiConst.XA_OK;
        }
    }
    
    /**
     * Map the C flags to the java XAResource flags.
     * Translate all flags, not?
     * @param cflags c flags
     * @return Java flags
     */
    int xa_xaresource_flags_map(long cflags) {
        int ret = 0;
        String flag = "";
        if ( (cflags & AtmiConst.TMJOIN) > 0) {
            
            ret |= XAResource.TMJOIN;
            flag = flag.concat("TMJOIN;");
        }
        
        if ( (cflags & AtmiConst.TMSUSPEND) > 0) {
            ret |= XAResource.TMSUSPEND;
            flag = flag.concat("TMSUSPEND;");
        }
        
        if ( (cflags & AtmiConst.TMSTARTRSCAN) > 0) {
            ret |= XAResource.TMSTARTRSCAN;
            flag = flag.concat("TMSTARTRSCAN;");
        }
        
        if ( (cflags & AtmiConst.TMENDRSCAN) > 0) {
            ret |= XAResource.TMENDRSCAN;
            flag = flag.concat("TMENDRSCAN;");
        }
        
        if ( (cflags & AtmiConst.TMSUCCESS) > 0) {
            ret |= XAResource.TMSUCCESS;
            flag = flag.concat("TMSUCCESS;");
        }
        
        if ( (cflags & AtmiConst.TMFAIL) > 0) {
            ret |= XAResource.TMFAIL;
            flag = flag.concat("TMFAIL;");
        }
        
        if (0==ret) {
            ret = XAResource.TMNOFLAGS;
            flag = flag.concat("TMNOFLAGS;");
        }
        
        tplogInfo("XA C flag %d mapped to Java %d (%s)", cflags, ret, flag);

        return ret;
    }
    
    /**
     * Map the java errors to c code
     * @param jerr java error code
     * @return  C error code
     */
    int xa_jerror_map(int jerr) {
        
        int ret;
        
        switch (jerr)
        {
            case XAException.XA_HEURCOM:
                ret = AtmiConst.XA_HEURCOM;
                break;
            case XAException.XA_HEURHAZ:
                ret = AtmiConst.XA_HEURHAZ;
                break;
            case XAException.XA_HEURMIX:
                ret = AtmiConst.XA_HEURMIX;
                break;
            case XAException.XA_HEURRB:
                ret = AtmiConst.XA_HEURRB;
                break;
            case XAException.XA_NOMIGRATE:
                ret = AtmiConst.XA_NOMIGRATE;
                break;
            case XAException.XA_RBBASE:
                ret = AtmiConst.XA_RBBASE;
                break;
            case XAException.XA_RBCOMMFAIL:
                ret = AtmiConst.XA_RBCOMMFAIL;
                break;
            case XAException.XA_RBDEADLOCK:
                ret = AtmiConst.XA_RBDEADLOCK;
                break;
            case XAException.XA_RBEND:
                ret = AtmiConst.XA_RBEND;
                break;
            case XAException.XA_RBINTEGRITY:
                ret = AtmiConst.XA_RBINTEGRITY;
                break;
            case XAException.XA_RBOTHER:
                ret = AtmiConst.XA_RBOTHER;
                break;
            case XAException.XA_RBPROTO:
                ret = AtmiConst.XA_RBPROTO;
                break;
            
            /*dup
                case XAException.XA_RBROLLBACK:
                ret = AtmiConst.XA_RBROLLBACK;
                break;
                */
            case XAException.XA_RBTIMEOUT:
                ret = AtmiConst.XA_RBTIMEOUT;
                break; 
                /*
                dup
            case XAException.XA_RBTRANSIENT:
                ret = AtmiConst.XA_RBTRANSIENT;
                break;
                */
                
            case XAException.XA_RDONLY:
                ret = AtmiConst.XA_RDONLY;
                break;
            case XAException.XA_RETRY:
                ret = AtmiConst.XA_RETRY;
                break;
            case XAException.XAER_ASYNC:
                ret = AtmiConst.XAER_ASYNC;
                break;
            case XAException.XAER_DUPID:
                ret = AtmiConst.XAER_DUPID;
                break;
            case XAException.XAER_INVAL:
                ret = AtmiConst.XAER_INVAL;
                break;
            case XAException.XAER_NOTA:
                ret = AtmiConst.XAER_NOTA;
                break;
            case XAException.XAER_OUTSIDE:
                ret = AtmiConst.XAER_OUTSIDE;
                break;
            case XAException.XAER_PROTO:
                ret = AtmiConst.XAER_PROTO;
                break;
            case XAException.XAER_RMFAIL:
                ret = AtmiConst.XAER_RMFAIL;
                break;
            default:
                ret = AtmiConst.XAER_RMERR;
                break;
        }
        
        tplogWarn("Java XA Error code %d mapped to %d c code", jerr, ret);
        
        return ret;
    }
    
    /**
     * Start transaction.
     * Check are we already in transaction? Or that will be checked by Enduro/X
     * @param xid
     * @param flags
     * @return XA_OK, XERR
     */
    int xa_start_entry(Xid xid, long flags) {
        
        int ret = xa_is_open();
        
        int jflags = xa_xaresource_flags_map(flags);
        
        if (ret!=AtmiConst.XA_OK)
        {
            tplogError("xa_start_entry: XA Not open, did you call tpopen()?");
            return ret;
        }
        
        /* start the tranaction */
        
        try {
            tplogInfo("format id %d", xid.getFormatId());
            tplogDump(AtmiConst.LOG_INFO, "xid getGlobalTransactionId", xid.getGlobalTransactionId());
            tplogDump(AtmiConst.LOG_INFO, "xid getBranchQualifier", xid.getBranchQualifier());
            
            xaRes.start(xid, jflags);
        } catch (XAException ex) {
            /* Log exception here */
            tplogex(AtmiConst.LOG_ERROR,"xa_start_entry got exception", ex);
            ret = xa_jerror_map(ex.errorCode);
        }
        return ret;
    }
    
    /**
     * Perform XA End
     * @param xid xid
     * @param flags flags
     * @return XA_OK, err
     */
    int xa_end_entry(Xid xid, long flags) {
        
        int ret = xa_is_open();
        
        int jflags = xa_xaresource_flags_map(flags);
        
        if (ret!=AtmiConst.XA_OK)
        {
            tplogError("xa_start_entry: XA Not open, did you call tpopen()?");
            return ret;
        }
        
        /* end xid session */
        
        try {
            
            //tplogInfo("jflags=%d", jflags);
            tplogInfo("format id %d", xid.getFormatId());
            tplogDump(AtmiConst.LOG_INFO, "xid getGlobalTransactionId", xid.getGlobalTransactionId());
            tplogDump(AtmiConst.LOG_INFO, "xid getBranchQualifier", xid.getBranchQualifier());
            //xaRes.end(xid, jflags);
            xaRes.end(xid, jflags);
            /*
            tplogInfo("About to prep..");
            xaRes.prepare(xid);
            tplogInfo("About to commit..");
            xaRes.commit(xid, false);
            */
        
        } catch (XAException ex) {
            /* ignore??? */
            /* Log exception here */
            tplogex(AtmiConst.LOG_ERROR,"xa_end_entry got exception", ex);
            //tplogError("Error code: %d", ex.)
            ret = xa_jerror_map(ex.errorCode);
            
        }
    
        return ret;
    }
    
    /**
     * Perform rollback
     * @param xid XID of transaction
     * @param flags not used
     * @return XA_OK, XAERR
     */
    int xa_rollback_entry(Xid xid, long flags) {
        
        int ret = xa_is_open();
        
        if (ret!=AtmiConst.XA_OK)
        {
            tplogError("xa_rollback_entry: XA Not open, did you call tpopen()?");
            return ret;
        }
        
        /* rollback transaction */
        
        try {
            xaRes.rollback(xid);
        } catch (XAException ex) {
            /* Log exception here */
            tplogex(AtmiConst.LOG_ERROR,"xa_rollback_entry got exception", ex);
            ret = xa_jerror_map(ex.errorCode);
        }
        return ret;
    }
    
    /**
     * Prepare transaction for commit
     * @param xid xid
     * @param flags Not used
     * @return XA_OK, XAERR
     */
    int xa_prepare_entry(Xid xid, long flags) {
        
        int ret = xa_is_open();
        
        if (ret!=AtmiConst.XA_OK)
        {
            tplogError("xa_prepare_entry: XA Not open, did you call tpopen()?");
            return ret;
        }
        
        /* prepare for commit */
        
        try {
            xaRes.prepare(xid);
        } catch (XAException ex) {
            /* Log exception here */
            tplogex(AtmiConst.LOG_ERROR,"xa_prepare_entry got exception", ex);
            ret = xa_jerror_map(ex.errorCode);
        }
        return ret;
    }
    
    /**
     * Commit transaction
     * @param xid xid
     * @param flags Not used
     * @return XA_OK, XAERR
     */
    int xa_commit_entry(Xid xid, long flags) {
        
        int ret = xa_is_open();
        
        if (ret!=AtmiConst.XA_OK)
        {
            tplogError("xa_commit_entry: XA Not open, did you call tpopen()?");
            return ret;
        }
        
        /* prepare for commit */
        
        try {
            xaRes.commit(xid, false);
        } catch (XAException ex) {
            /* Log exception here */
            tplogex(AtmiConst.LOG_ERROR,"xa_commit_entry got exception", ex);
            ret = xa_jerror_map(ex.errorCode);
        }
        
        return ret;
    }
    
    /**
     * Commit transaction
     * @param xid xid
     * @param flags Not used
     * @return XA_OK, XAERR
     */
    int xa_forget_entry(Xid xid, long flags) {
        
        int ret = xa_is_open();
        
        
        if (ret!=AtmiConst.XA_OK)
        {
            tplogError("xa_forget_entry: XA Not open, did you call tpopen()?");
            return ret;
        }
        
        /* prepare for commit */
        
        try {
            xaRes.forget(xid);
        } catch (XAException ex) {
            /* Log exception here */
            tplogex(AtmiConst.LOG_ERROR,"xa_forget_entry got exception", ex);
            ret = xa_jerror_map(ex.errorCode);
        }
        
        return ret;
    }
    
    /**
     * Recover transactions
     * @param flags recover flags
     * @return List or error
     */
    XidList xa_recover_entry(long flags) {
        
        XidList ret = new XidList();
        int jflags = xa_xaresource_flags_map(flags);
        
        ret.ret = xa_is_open();
        
        if (ret.ret!=AtmiConst.XA_OK)
        {
            tplogError("xa_recover_entry: XA Not open, did you call tpopen()?");
            return ret;
        }
        
        try {
            ret.list = xaRes.recover(jflags);
        } catch (XAException ex) {
            /* Log exception here */
            tplogex(AtmiConst.LOG_ERROR,"xa_forget_entry got exception", ex);
            ret.ret = xa_jerror_map(ex.errorCode);
        }
        
        return ret;
        
    }

    /**
     * Server threads processing. From the dispatcher thread XATMI call handle
     * can be captured and delivered to worker thread. The handle must be
     * freed by tpsrvfreectxdata().
     * 
     * @defgroup svthreads Server threads
     * @{
     */
    
    /**
     * Get server context data for multi threaded server
     * See tpsrvgetctxdata(3) manpage for more information.
     * 
     * @throws  AtmiTPEPROTOException Global transaction was started and it was 
     *  marked for abort-only, there was any open call descriptors 
     *  with-in global transaction,
     * 
     * @throws AtmiTPERMERRException Global transaction was started and it was 
     *  marked for abort-only, there was any open call descriptors 
     *  with-in global transaction
     * 
     * @throws AtmiTPESYSTEMException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info. This could also be a 
     *  problem with dynamical driver loading.
     * 
     * @throws AtmiTPEOSException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info.
     * 
     * @return C pointer to allocated server context data
     */
    public native long tpsrvgetctxdata();
    
    /**
     * Associate current thread with server call data.
     * See tpsrvsetctxdata(3) manpage for more information.
     * @param flags[in] for server worker threads use AtmiConst.SYS_SRV_THREAD
     */
    public native void tpsrvsetctxdata(long dataptr, long flags);
    
    /**
     * Free Server context data
     * @param dataptr pointer returned by \r tpsrvgetctxdata()
     */
    public native void tpsrvfreectxdata(long dataptr);
    
    /**
     * Continue the server work after extracting the context data
     */
    public native void tpcontinue();
    
    /** @} */ // end of DTran
    

    /**
     * Distributed transaction processing
     * @defgroup DTran Distributed transaction processing methods
     * @{
     */
    
    /**
     * Open the XA sub-system / open connection for ATMI Context.
     * If Enduro/X Java XA Driver is used, then it will prepare connection
     * connection for given Atmi Context.
     * 
     * See tpopen(3) manpage for more information.
     * 
     * @throws  AtmiTPERMERRException Resource Manager failed. The tpstrerror() 
     *  will provide more info from last call.
     * @throws  AtmiTPESYSTEMException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info. This could also be a 
     *  problem with dynamical driver loading.
     * @throws  AtmiTPEOSException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info.
     */
    public native void tpopen();
    
    /**
     * Close XA sub-system.
     * This shall be called after the processing thread terminates.
     * 
     * See tpopen(3) manpage for more information.
     * 
     * @throws  AtmiTPERMERRException Resource Manager failed. The tpstrerror() 
     *  will provide more info from last call.
     * @throws  AtmiTPESYSTEMException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info. This could also be a 
     *  problem with dynamical driver loading.
     * @throws  AtmiTPEOSException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info.
     */
    public native void tpclose();
    
    /**
     * Begin the global transaction. The current user must be open XA subsystem 
     * by topopen(). timeout is setting for transaction manager (tmsrv) how 
     * long transaction can hang in active state (i.e. not prepared/committed). 
     * The work done by tpbegin() transaction is only in scope of processes 
     * which uses XA interfaces for resource managers. 
     * Any others works are outside of current scope. If timeout is set to 0, 
     * then default maximum time for transaction processing is used. 
     * Transaction must be committed or aborted by client process by using 
     * tpcommit() or tpabort(). If any tpcall() with no TPNOTRAN flag 
     * fails within given global transaction, then transaction is automatically 
     * marked as abort only.
     * See tpopen(3) manpage for more information.
     * 
     * @param timeout Number of seconds for which transaction can spend
     *  in active state. If time is overreached, transaction is aborted by
     * tmsrv(8).
     * @param flags. Reserved for future use. Currently must be set to 0.
     * @throws  AtmiTPEINVALException flags was not 0.
     * @throws  AtmiTPETIMEException Transaction manager (tmsrv(8)) did not 
     *    respond in configured time-out time. The state of transaction is unknown.
     * @throws  AtmiTPESVCERRException Failed to call transaction manager, 
     *  with service error. The state of transaction is unknown.
     * @throws  AtmiTPEPROTOException XA subsystem was not initialized 
     *  (did not call tpopen()) or global transaction already started.
     * @throws  AtmiTPESYSTEMException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info.
     * @throws  AtmiTPEOSException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info. 
     */
    public native void tpbegin (long timeout, long flags);
    
    /**
     * Function does commit the global transaction. Transaction must not be 
     * marked as abort only (e.g. in case if tpcall() failed). 
     * After issuing the command, tmsrv(8) will do the commit procedure, 
     * prepare phase first, log the results of prepare to persistent storage, 
     * and then do the actual commit.
     * @param flags 0 or Atmiconst.TPTXCOMMITDLOG - for logged decision early
     *  return
     * @throws  AtmiTPEINVALException flags was not 0 or was not TPTXCOMMITDLOG.
     * @throws  AtmiTPETIMEException Transaction manager (tmsrv(8)) did not 
     *  respond in configured time-out time. The state of transaction is unknown.
     * @throws  AtmiTPEABORTException Global transaction was marked for abort 
     *  and was aborted, or prepare state failed for some of the 
     *  resource managers and transaction was aborted.
     * @throws  AtmiTPEHAZARDException The state of transaction is not fully know. 
     *  It can be that it is partially committed and partially aborted.
     * @throws  AtmiTPEHEURISTICException The state of transaction is not 
     *  full known. The transaction heuristically completed.
     * @throws  AtmiTPESVCERRException Failed to call transaction manager, 
     *  with service error. The state of transaction is unknown.
     * @throws  AtmiTPEPROTOException XA subsystem was not initialized 
     *  (did not call tpopen()), no global transaction started or caller 
     *  is not initiator of transaction.
     * @throws  AtmiTPESYSTEMException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info.
     * @throws  AtmiTPEOSException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info. 
     */
    public native void tpcommit (long flags);
    
    /**
     * Abort the global transaction. This will make any resources touched during 
     * the transaction to roll back. Enduro/X will make all resource managers 
     * to rollback the changes. Abort of transaction can only be done by transaction
     * initiator, who originally did tpbegin().
     * @param flags is reserved for future use and must be 0.
     * @throws AtmiTPEINVALException flags was not 0.
     * @throws AtmiTPETIMEException Transaction manager (tmsrv(8)) did not 
     *  respond in configured time-out time. The state of transaction is unknown.
     * @throws AtmiTPEHAZARDException The state of transaction is not fully know. 
     *  It can be that it is partially committed and partially aborted.
     * @throws AtmiTPEHEURISTICException The state of transaction is not full 
     *  known. The transaction heuristically completed.
     * @throws AtmiTPESVCERRException Failed to call transaction manager, 
     *  with service error. The state of transaction is unknown.
     * @throws AtmiTPEPROTOException XA subsystem was not initialized (did not 
     *  call tpopen()), no global transaction started or caller is not 
     *  initiator of transaction.
     * @throws AtmiTPESYSTEMException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info.
     * @throws AtmiTPEOSException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info.
     */
    public native void tpabort (long flags);
    
    /**
     * Disassociate XA transaction from context and store transaction details 
     * in returned TPRANID object. The transaction can be resumed as by doing 
     * tpresume(). tranid must not be NULL. flags are reserved for future use, 
     * and currently must be set to 0. It is error to have any open 
     * call descriptors (either by tpacall() or by tpconnect()). Suspending 
     * can be done by any involved process in transaction. But the role of 
     * participant does not change. The TPTRANID type records the information 
     * about caller, is it transaction originator or not.
     * @param flags reserved for future use and must be be *0*
     * @return allocated transaction identifier
     * @throws AtmiTPEINVALException flags was not 0. Or tranid was NULL.
     * @throws AtmiTPEPROTOException XA subsystem was not initialized (did not 
     *  call tpopen()), global transaction was not already started, or there 
     *  was open call descriptors.
     * @throws AtmiTPESYSTEMException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info.
     * @throws AtmiTPEOSException System failure occurred during serving. 
     * See logs i.e. user log, or debugs for more info.
     */
    public native TPTRANID tpsuspend (long flags);
    
    /**
     * Resume global transaction with given transaction data in tranid parameter. 
     * trandid previously must be set by tpsuspend() call. To resume global 
     * transaction, XA sub-system must be open, by tpopen(). Technically 
     * it is possible that resume is done by other process than which 
     * did the suspend
     * @param tid transaction id previosuly returned from \r tpsuspend() call.
     * @param flags must be set to 0 (RFU).
     * @throws AtmiTPEINVALException Flags was not 0. Or tranid was NULL.
     * @throws AtmiTPEPROTOException Global transaction is already started.
     * @throws AtmiTPESYSTEMException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info. Could be the case if 
     *  XA sub-system is not open by tpopen().
     * @throws AtmiTPEOSException System failure occurred during serving. 
     *  See logs i.e. user log, or debugs for more info.
     */
    public native void tpresume (TPTRANID tid, long flags);
    
    /**
     * Get database connection
     * It is up to user to close the connections.
     * @return DB connection, null in case if there was no tpopen(), or there was tpclose().
     */
    public Connection tpgetconn() throws SQLException {
        return xaConn.getConnection();
    }
    
    /**
     * Return global transaction level
     * @return 0 - current thread is not part of global transaction. 
     *  1 - current thread is part of global transaction.
     */
    public native int tpgetlev();
    
    
    /** @} */ // end of DTran
    
    /**
     * Create ATMI Context by static call.
     * @param ctx C pointer
     * @return Atmi Context Object
     */
    public static AtmiCtx createAtmiCtx(long ctx) {
        return new AtmiCtx(ctx);
    }
    
    /**
     * This set a callback method indicating that shutdown 
     * is initiated from the OS. This captures the SIGTERM, SIGINT, SIGHUP
     * signals. I.e. hides from the Java. On first method call, new signals
     * are masked, and new thread is started which monitors the arrival of
     * these signals. Once the signal is detected, the \r run object is executed.
     * @param run object to be executed (if set) in case if
     * termination signal is received. In case of NULL, the signal thread is
     * not terminated, but it will do nothing in case of receiving the shutdown
     * signals.
     */
    public native void installTermSigHandler(java.lang.Runnable run);
}

/* vim: set ts=4 sw=4 et smartindent: */
