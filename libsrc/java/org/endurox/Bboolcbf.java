/**
 * @brief Interface for callback
 *
 * @interface Bboolcbf
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


/**
 * Interface for boolean expression callback
 */
public interface Bboolcbf  {
    
    /**
     * Callback function from the boolean expression evaluator
     * @param ctx ATMI Context (from UBF buffer)
     * @param ub UBF buffer on to which evaluate is done
     * @param funcname function name called
     * @return value to be injected back into expression
     */
    public long bboolCallBack(AtmiCtx ctx, TypedUBF ub, String funcname);
    
}
/* vim: set ts=4 sw=4 et smartindent: */