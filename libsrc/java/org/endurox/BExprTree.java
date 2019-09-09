/**
 * @brief UBF Compiled expression handler
 *
 * @class BExprTree
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
 * UBF Buffer compiled boolean expressions handler
 */
public class BExprTree  {
    
    /** ptr to char *tree */
    long cPtr = 0;
    
    
    /** ATMI Context by which expression was allocated 
    AtmiCtx ctx;*/
    
    /**
     * Deallocate the tree
     * @param cPtr 
     */
    native void Btreefree(long cPtr);
    
    /**
     * Package level visibility
     * @param cPtr C pointer to compiled boolean expression
     */
    BExprTree(long cPtr) {
        this.cPtr = cPtr;
    }
    
    @Override
    protected void finalize() throws Throwable {

        cleanup();
        //Remove expression object
        super.finalize();
    }
    
    /**
     * Clean up the object (basically this is destructor) as we cannot relay
     * on finalize() auto call of from the Garbage Collector. Thus we object
     * goes out of the scope,
     */
    public void cleanup() {
        
        if (0!=cPtr) {
            Btreefree(cPtr);
        }
        cPtr = 0;
    }
    
}
/* vim: set ts=4 sw=4 et smartindent: */
