/**
 * @brief Enduro/X Queue Control class. Contains details about message
 *  to be enqueued, or information about message dequeued.
 *
 * @class TPQCTL
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
 * Enduro/X Queue Message interface.
 * Holds infos about message to be enqueued or dequeued
 */
public class TPQCTL {
	
    long flags;             /**< indicates which of the values are set */
    long deq_time;          /**< absolute/relative  time for dequeuing */
    long priority;          /**< enqueue priority */
    long diagnostic;        /**< indicates reason for failure */
    String diagmsg;         /**< diagnostic message */
    /** id of message before which to queue */
    byte []msgid = new byte[AtmiConst.TMMSGIDLEN];
    /** correlation id used to identify message */
    byte []corrid = new byte [AtmiConst.TMCORRIDLEN];
    String replyqueue;      /**< queue name for reply message */
    String failurequeue;    /**< queue name for failure message */
    ClientId cltid;         /**< client identifier for originating client */
    long urcode;            /**< application user-return code */
    long appkey;            /**< application authentication client key */
    long delivery_qos;      /**< delivery quality of service  */
    long reply_qos;         /**< reply message quality of service  */
    long exp_time;          /**< expiration time  */
}

/* vim: set ts=4 sw=4 et smartindent: */
