/**
 * @brief ATMI Buffer class (base class common for all buffers)
 *
 * @class TypedBuffer
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
package org.endurox;

/**
 * This is data type buffer for XATMI IPC.
 * Note that only one instance of the buffer may hold the destructor for C
 * counter parts. Some buffers (received by service call) may not be even
 * associated by destructor, as those are dellocated by XATMI sub-system.
 */
public class TypedBuffer {
	
    /**
     * Pointer to C ATMI Context object
     */
    AtmiCtx ctx;
    
    /**
     * Shall we destruct the linked C side ATMI object.
     * No need to collect the object for auto buffers.
     */
    boolean doFinalize;
    
    /* Allow access from package: 
     * TODO: well here we need a small object which will be shared between
     * return types for same object. And this small object will have a destructor.
     * this will ensure that destructor will happen only when all referes will
     * be deleted.
     */
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
     * Initialize ATMI Buffer Object
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
     * Change the owner ATMi Context, when buffer is passed from one context /
     * thread to another
     * @throws NullPointerException ctx is null
     * @param ctx ATMI Context
     */
    public void setAtmiCtx(AtmiCtx ctx) {
        
        if (null==ctx) {
            throw new NullPointerException("ctx must not be NULL");
        }
        
        this.ctx = ctx;
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
    
    /**
     * Get the buffer type description from middleware point of view
     * @return Type descriptor
     */
    public native TpTypesResult tptypes();    
    
    static {
       System.loadLibrary("exjavald"); // Load native library at runtime
    }
}
/* vim: set ts=4 sw=4 et smartindent: */
