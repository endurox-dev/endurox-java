/**
 * @brief ATMI Buffer class (base class common for all buffers)
 *
 * @class TypedBuffer
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

public class TypedBuffer {
	
    /**
     * Pointer to C ATMI Context object
     */
    AtmiCtx ctx;
    boolean doFinalize;
    
    /* Allow access from package: */
    long cPtr;
    long len;

    /**
     * Free up the given buffer
     * @param[in] cPtr C pointer to buffer
     */
    private native void tpfree (long cPtr);
    
    /**
     * Reallocate ATMI buffer
     * @param size[in] New buffer size
     * @throws AtmiTPEOSException Operating system exception occurred
     */
    public native void tprealloc(long size);

    /**
     * Initialize ATMI Object
     * @param ctx[in] ATMI Context allocated this method
     * @param doFinalize[in] Should
     * @param cPtr[in] C pointer to allocated block
     * @param len[in] Conditional buffer length
     */
    public TypedBuffer(AtmiCtx ctx, boolean doFinalize, long cPtr, long len) {
         this.ctx = ctx;
         this.doFinalize = doFinalize;
         this.cPtr = cPtr;
         this.len = len;
    }

    /**
     * Clean up the object (basically this is destructor) as we cannot relay
     * on finalize() auto call of from the Garbage Collector. Thus we object
     * goes out of the scope,
     */
    public void cleanup() {
        
        ctx = null;
        if (doFinalize && 0!=cPtr) {
            tpfree(cPtr);
            cPtr = 0;
        }
    }
    
    /**
     * Finish of this ATMI buffer
     */
    @Override
    protected void finalize() throws Throwable{
        
        cleanup();
        super.finalize();
    }
    
    /**
     * Set the finalize flag
     */
    public void setDoFinalize(boolean b) {
        doFinalize = b;
    }
    
    
    static {
       System.loadLibrary("exjava"); // Load native library at runtime
    }
}
/* vim: set ts=4 sw=4 et smartindent: */
