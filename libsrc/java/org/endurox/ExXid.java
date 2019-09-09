/**
 * @brief Enduro/X XID implementation
 *
 * @class ExXid
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

import java.util.Arrays;
import javax.transaction.xa.Xid;

/**
 * Enduro/X version of transaction XID
 * @author mvitolin
 */
public class ExXid implements Xid {
    
    /** Full C data */
    byte [] data = null;
    
    /** Transaction ID */
    byte [] trxid = null;
    /** Branch qualifier */
    byte [] bqual = null;
    
    /** Format ID */
    long formatID;
    
    long gtrid_length; /**< value from 1 through 64 */
    long bqual_length; /**< value from 1 through 64 */

    
    /**
     * Create Transaction XID
     * @param formatID format id code
     * @param gtrid_length transaction id portion length
     * @param bqual_length branch qualifier length 
     * @param data data bytes containing trxid and bqual
     */
    public ExXid(long formatID, long gtrid_length, long bqual_length, byte [] data) {
        
        this.gtrid_length = gtrid_length;
        this.bqual_length = bqual_length;
        this.formatID = formatID;
        this.data = new byte[data.length];
        
        System.arraycopy(data, 0, this.data, 0, data.length);
        
        trxid = new byte[(int)gtrid_length];
        
        /* extract the trxid poritions */
        System.arraycopy(data, 0, trxid, 0, (int)gtrid_length);
        
        bqual = new byte[(int)bqual_length];
        
        /* extract the bqual poritions */
        System.arraycopy(data, (int)gtrid_length, bqual, 0, (int)bqual_length);
        
    }
    
    /**
     * Get format ID of xid
     * @return format id code
     */
    public int getFormatId() {
        return (int)formatID;
    }

    /**
     * Get global transaction id
     * @return transaction id portion
     */
    public byte[] getGlobalTransactionId() {
        return trxid;
    }

    /**
     * Get branch qualifier
     * @return qualifier portion
     */
    public byte[] getBranchQualifier() {
        return bqual;
    }
    
    /**
     * required for compare
     * @return 
     */
    @Override
    public int hashCode() {
        final int prime = 31;
        int result = 1;
        result = prime * result + Arrays.hashCode(bqual);
        result = prime * result + (int)formatID;
        result = prime * result + Arrays.hashCode(trxid);
        return result;
    }

    /**
     * required for compare
     * @param obj
     * @return 
     */
    @Override
    public boolean equals(Object obj) {
        if (this == obj)
            return true;
        if (obj == null)
            return false;
        if (getClass() != obj.getClass())
            return false;
        ExXid other = (ExXid) obj;
        if (!Arrays.equals(bqual, other.bqual))
            return false;
        if (formatID != other.formatID)
            return false;
        if (!Arrays.equals(trxid, other.trxid))
            return false;
        return true;
    }

    
}

/* vim: set ts=4 sw=4 et smartindent: */
