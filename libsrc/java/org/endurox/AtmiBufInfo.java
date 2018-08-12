/**
 * @class AtmiBufInfo
 *
 * @brief Buffer information (return from tpTypes)
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
package org.endurox;

public class AtmiBufInfo {

    /**
     * Buffer type
     */
    String type;
    
    /**
     * Buffer sub type
     */
    String subType;
    
    /**
     * Buffer size
     */
    long size;

    /**
     * Get buffer size
     * @return buffer size in bytes
     */
    public long getSize() {
        return size;
    }

    /**
     * ATMI Buffer type
     * @return buffer type code
     */
    public String getType() {
        return type;
    }

    /**
     * ATMI Buffer sub-type
     * @return buffer sub-type string
     */
     public String getSubType() {
        return subType;
    }
     
    /**
     * Instantiate Buffer Info object
     * @param type buffer type code
     * @param subType sub-type
     * @param size size in bytes
     */
    public AtmiBufInfo(String type, String subType, long size) {
        this.type = type;
        this.subType = subType;
        this.size = size;
    }

}

