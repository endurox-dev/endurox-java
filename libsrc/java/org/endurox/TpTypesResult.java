/**
 * @brief Return type information for give XATMI buffer
 *
 * @class TpTypesResult
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
 * Result value from AtmiCtx.tptypes() call
 */
public class TpTypesResult {
    
    /**
     * Buffer type code
     */
    String type;
    
    /**
     * Buffer sub-type code (used for views)
     */
    String subType;
    
    /**
     * Buffer size in bytes
     */
    long size;

    /**
     * Allocate types result
     * @param type buffer type code
     * @param subType  sub-type
     */
    public TpTypesResult(String type, String subType, long size) {
        this.type = type;
        this.subType = subType;
        this.size = size;
    }

    /**
     * Get buffer type
     * @return buffer type code one of these: NULL, TPINIT, UBF, JSON, STRING, CARRAY, VIEW
     */
    public String getType() {
        return type;
    }

    /**
     * Buffer sub-type, i.e. view name
     * @return view name for buffer type "VIEW", for others it is empty string
     */
    public String getSubType() {
        return subType;
    }

    /**
     * Get allocated buffer size
     * @return buffer size in bytes
     */
    public long getSize() {
        return size;
    }
    
}

/* vim: set ts=4 sw=4 et smartindent: */
