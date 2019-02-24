/**
 * @brief tprecv result
 *
 * @class TprecvResult
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
 * AtmiCtx.tprecv() return type
 * Conversation receive return type
 */
public class TprecvResult {

    /**
     * Return event
     */
    long revent;
    
    /**
     * Call descriptor returned
     */
    int cd;
    
    /**
     * Associated buffer returned from call
     */
    TypedBuffer buffer;

    /**
     * Create instance of tprecv results 
     * @param cd conversation descriptor
     * @param buffer Typed buffer received
     * @param revent Return event code
     */
    public TprecvResult(int cd, TypedBuffer buffer, long revent) {
        this.cd = cd;
        this.buffer = buffer;
        this.revent = revent;
    }

    /**
     * Get conversation descriptor
     * @return conversation descriptor
     */
    public int getCd() {
        return cd;
    }

    /**
     * XATMI buffer associated with receive
     * @return XATMI Buffer
     */
    public TypedBuffer getBuffer() {
        return buffer;
    }

    /**
     * Returned event type 
     * @return event code, see "TPEV_" from AtmiConst.
     */
    public long getRevent() {
        return revent;
    }
    
}
/* vim: set ts=4 sw=4 et smartindent: */
