package org.endurox;
/**
 * @class BFldLocInfo
 *
 * @brief Fast add field location information. Holds the pointer to last added
 *  field
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
public class BFldLocInfo {

    /**
     * C pointer to last checked field (casted to BFLDID * at C side)
     */
    long cPtr_last_checked;
    
    /**
     * Create the location field field. When object is created 
     */
    public BFldLocInfo() {
        this.cPtr_last_checked = 0;
    }
    
    /**
     * Reset location field so that we can fast add again
     */
    public void Reset() {
        this.cPtr_last_checked = 0;
    }
}
