/**
 * @class TpSvcInfo
 *
 * @brief Service info block
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

/**
 * Service call information
 */
public class TpSvcInfo {
    
    /** Service name */
    String  name;
    
    /** ATMI data buffer */
    TypedBuffer data;
    
    /** Call flags */
    long    flags;
    
    /** Call descriptor */
    int     cd;
    
    /** RFU */
    long    appkey;
    
    /** Client ID */
    ClientId cltId;
    
    /** Invoked function name */
    String  fname;

    /** @return Called service name */
    public String getName() {
        return name;
    }

    /** @return Call data */
    public TypedBuffer getData() {
        return data;
    }

    /** @return call flags */
    public long getFlags() {
        return flags;
    }

    /** @return call descriptor */
    public int getCd() {
        return cd;
    }

    /**@return RFU */
    public long getAppkey() {
        return appkey;
    }

    /** @return Get client ID */
    public ClientId getCltId() {
        return cltId;
    }

    /** @return Get function invoked name  */
    public String getFname() {
        return fname;
    }
    
    /**
     * Create Service call info object
     * @param name service name
     * @param data call data
     * @param flags call flags
     * @param cd    Call descriptor
     * @param appkey    RFU
     * @param cltId client id object
     * @param fname function name invoked
     */
    public TpSvcInfo(String name, TypedBuffer data, long flags, int cd, long appkey, 
            ClientId cltId, String fname) {
        this.name = name;
        this.data = data;
        this.flags = flags;
        this.cd = cd;
        this.appkey = appkey;
        this.cltId = cltId;
        this.fname = fname;
    }
}
