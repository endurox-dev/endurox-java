/**
 * @brief Stop watch implementation
 *
 * @class StopWatch
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
 */
public class StopWatch {
	
    private long t;
    
    /**
     * Initializes a new stopwatch.
     */
    public StopWatch() {
        reset();
    } 
    
    /**
     * Reset Stopwatch
     */
    public void reset() {
        t = System.currentTimeMillis();
    }
    
    /**
     * Return delta spent from constructor or from last reset
     * @return milliseconds from reset
     */
    public long delta() {
        long now = System.currentTimeMillis();
        
        return now-t;
    }
    
    /**
     * Return delta spent in seconds from last reset
     * @return seconds from last reset
     */
    public long deltaSec() {
        return delta() / 1000;
    }
    
}

/* vim: set ts=4 sw=4 et smartindent: */
