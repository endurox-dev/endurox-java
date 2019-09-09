/**
 * @brief Typed Byte array buffer format (C Array)
 *
 * @class TypedCarray
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

/**
 * C Byte array buffer.
 * The length handling strategy is following:
 * - The actual length is stored in TypedBuffer.len
 * - When allocating from Java side, the TypedBuffer.len holds the infos about actual
 *  data length
 * - When buffer is received from XATMI dispatcher / service call, also the
 * actual data length is installed here in java.
 */
public class TypedCarray extends TypedBuffer {
	
   /**
    * Initialize Carray buffer
    * @param ctx[in] ATMI Context allocated this method
    * @param doFinalize[in] Should the buffer be deallocated automatically
    * @param cPtr[in] C pointer to allocated block
    * @param len[in] Conditional buffer length
    */
   public TypedCarray(AtmiCtx ctx, boolean doFinalize, long cPtr, long len) {
        super(ctx, doFinalize, cPtr, len);
   }
   
   /**
    * Set C byte array value
    * @param b byte array to set value
    */
   public native void setBytes(byte [] b);
   
   
   /**
    * Get C array value
    * @return Byte array
    */
   public native byte[] getBytes();
   
}

/* vim: set ts=4 sw=4 et smartindent: */
