package org.endurox;
/**
 * @class AtmiCtx
 *
 * @brief Enduro/X ATMI Context Class
 *  This is main class used for almost all operations with Enduro/X middleware
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

public class AtmiCtx {
	
    /**
     * Pointer to C ATMI Context object
     */
    private long ctx = 0;

    static {
       System.loadLibrary("exjava"); // Load native library at runtime
                            // hello.dll (Windows) or libenduroxjava.so (Unixes)
    }
    
    /* TODO: We need a registry with non terminated ATMI contexts
     * so that we can hook up the JVM and remove all open contexts at
     * shutdown 
     */

    /**
     * Get ATMI Error 
     * @return Error tuple (code and message)
     */
    private native ErrorTuple getAtmiError();

    /**
     * Allocate new ATMI Context
     * ptr to ATMI context
     * @return C pointer to context object
     * @exception AtmiTPESYSTEMException
     */
    private static native long tpnewctxt();

    /**
     * Free up ATMI Context
     * @param ctx ATMI Context
     */
    private static native void tpfreectxt(long ctx);

    /**
     * Allocate buffer
     * Thow some exceptions.. too!
     * @param btype buffer type name
     * @param bsubtype buffer sub type or empty string (if no subtype)
     * @param size buffer size in bytes
     * @return allocate ATMI buffer
     */
    public native AtmiBuf tpAlloc(String btype, String bsubtype, long size);

    /**
     * Allocate new ATMI Context
     */
    public AtmiCtx()  {
        // This thorws TPESYSTEM if failed.
       ctx = tpnewctxt();
    }
     
    /**
     * Terminate the ATMI Context
     * @throws Throwable 
     */
    @Override
    protected void finalize() throws Throwable {

       if (0x0 != ctx)
       {
          tpfreectxt(ctx);
       }

       //Remove ATMI context...
       super.finalize();
    }
     
    // Test Driver
    public static void main(String[] args) {
     AtmiCtx ctx = new AtmiCtx();

     TypedUbf b = (TypedUbf)ctx.tpAlloc("UBF", "", 1024);

     System.out.printf("Buffer OK!");

     b = null;

     System.gc();

     ErrorTuple err = ctx.getAtmiError();
       System.out.printf("%d: %s\n", err.err, err.msg);

    }

}

