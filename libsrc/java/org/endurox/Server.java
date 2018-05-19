package org.endurox;
/**
 * @interface Server
 *
 * @brief Server standard startup and shutdown interface
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
public interface Server {
    /**
     * Server init call
     * @param ctx ATMI Context into which server is started
     * @param argv Enduro/X command line options
     * @return 0 in case of success, -1 failed
     */
    public int tpSvrInit(AtmiCtx ctx, String [] argv);
    
    /**
     * Server call when shutdown of server process is performed
     * @param ctx ATMI Context into which server is started
     */
    public void tpSvrDone(AtmiCtx ctx);
}
