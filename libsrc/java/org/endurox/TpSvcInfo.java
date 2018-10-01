/**
 * @brief Service info block
 *
 * @class TpSvcInfo
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
/* vim: set ts=4 sw=4 et smartindent: */
