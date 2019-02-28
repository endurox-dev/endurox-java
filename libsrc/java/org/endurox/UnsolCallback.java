/**
 * @brief Interface for unsoliced callback message
 *
 * @interface UnsolCallback
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
 * Interface for unsolicited callback message 
 */
public interface UnsolCallback  {
    
    /**
     * Process have receive unsolicited message from the XATMI system.
     * The functionality that can be handled within function is limited to:
     * - Buffer allocation/free
     * - tpgetlev()
     * - tptypes()
     * To have some more extended ATMI processing here, new context & thread
     * shall be allocated and buffer shall be copied to newly allocated, that
     * should be passed to thread.
     * @param ctx Atmi Context
     * @param buf Typed buffer
     * @param flags flags
     */
    public void unsolCallback(AtmiCtx ctx, TypedBuffer buf, long flags);
    
}
/* vim: set ts=4 sw=4 et smartindent: */
