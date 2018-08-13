/**
 * @brief Init class for ATMI client initialization. Currently reserved for
 *   future use only.
 *
 * @class TpInit
 */
/* -----------------------------------------------------------------------------
 * Enduro/X Middleware Platform for Distributed Transaction Processing
 * Copyright (C) 2009-2016, ATR Baltic, Ltd. All Rights Reserved.
 * Copyright (C) 2017-2018, Mavimax, Ltd. All Rights Reserved.
 * This software is released under one of the following licenses:
 * GPL or Mavimax's license for commercial use.
 * -----------------------------------------------------------------------------
 * GPL license:
 * 
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 3 of the License, or (at your option) any later
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
 
public class TpInit {
    
    String cltname;
    String passwd;
    String grpname;
    long flags;
    long datalen;
    long data;
    
    /**
     * RFU
     */
    String usrname;

    /**
     * RFU
     */
    public String getUsrname() {
        return usrname;
    }

    /**
     * RFU
     */
    public void setUsrname(String usrname) {
        this.usrname = usrname;
    }

    /**
     * RFU
     */
    public String getCltname() {
        return cltname;
    }

    /**
     * RFU
     */
    public void setCltname(String cltname) {
        this.cltname = cltname;
    }

    /**
     * RFU
     */
    public String getPasswd() {
        return passwd;
    }

    /**
     * RFU
     */
    public void setPasswd(String passwd) {
        this.passwd = passwd;
    }

    /**
     * RFU
     */
    public String getGrpname() {
        return grpname;
    }

    /**
     * RFU
     */
    public void setGrpname(String grpname) {
        this.grpname = grpname;
    }

    /**
     * RFU
     */
    public long getFlags() {
        return flags;
    }

    /**
     * RFU
     */
    public void setFlags(long flags) {
        this.flags = flags;
    }
    
    /**
     * RFU
     */
    public long getDatalen() {
        return datalen;
    }
    
    /**
     * RFU
     */    
    public void setDatalen(long datalen) {
        this.datalen = datalen;
    }

    /**
     * RFU
     */    
    public long getData() {
        return data;
    }

    /**
     * RFU
     */    
    public void setData(long data) {
        this.data = data;
    }
    
    /**
     * Instantiate TpInit object (RFU)
     * @param usrname RFU
     * @param cltname RFU
     * @param passwd RFU
     * @param grpname RFU
     * @param flags RFU
     * @param datalen RFU
     * @param data  RFU
     */
    public TpInit(String usrname, String cltname, String passwd, String grpname, 
            long flags, long datalen, long data) {
        this.usrname = usrname;
        this.cltname = cltname;
        this.passwd = passwd;
        this.grpname = grpname;
        this.flags = flags;
        this.datalen = datalen;
        this.data = data;
    }
    
}

/* vim: set ts=4 sw=4 et smartindent: */
