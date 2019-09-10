/**
 * @brief Enduro/X Queue Control class. Contains details about message
 *   to be enqueued, or information about message dequeued.
 *
 * @class TPQCTL
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

/**
 * Enduro/X message queue interface.
 * Holds infos about message to be enqueued or dequeued
 */
public class TPQCTL {

    public long getFlags() {
        return flags;
    }

    public void setFlags(long flags) {
        this.flags = flags;
    }

    public long getDeq_time() {
        return deq_time;
    }

    public void setDeq_time(long deq_time) {
        this.deq_time = deq_time;
    }

    public long getPriority() {
        return priority;
    }

    public void setPriority(long priority) {
        this.priority = priority;
    }

    public long getDiagnostic() {
        return diagnostic;
    }

    public void setDiagnostic(long diagnostic) {
        this.diagnostic = diagnostic;
    }

    public String getDiagmsg() {
        return diagmsg;
    }

    public void setDiagmsg(String diagmsg) {
        this.diagmsg = diagmsg;
    }

    public byte[] getMsgid() {
        return msgid;
    }

    public void setMsgid(byte[] msgid) {
        //this.msgid = msgid;
        this.msgid = Arrays.copyOf(msgid, msgid.length);
    }

    public byte[] getCorrid() {
        return corrid;
    }

    public void setCorrid(byte[] corrid) {
        /*
        this.corrid = corrid;
        */
        /* copy off!!! */
        this.corrid = Arrays.copyOf(corrid, corrid.length);
    }

    public String getReplyqueue() {
        return replyqueue;
    }

    public void setReplyqueue(String replyqueue) {
        this.replyqueue = replyqueue;
    }

    public String getFailurequeue() {
        return failurequeue;
    }

    public void setFailurequeue(String failurequeue) {
        this.failurequeue = failurequeue;
    }

    public ClientId getCltid() {
        return cltid;
    }

    public void setCltid(ClientId cltid) {
        this.cltid = cltid;
    }

    public long getUrcode() {
        return urcode;
    }

    public void setUrcode(long urcode) {
        this.urcode = urcode;
    }

    public long getAppkey() {
        return appkey;
    }

    public void setAppkey(long appkey) {
        this.appkey = appkey;
    }

    public long getDelivery_qos() {
        return delivery_qos;
    }

    public void setDelivery_qos(long delivery_qos) {
        this.delivery_qos = delivery_qos;
    }

    public long getReply_qos() {
        return reply_qos;
    }

    public void setReply_qos(long reply_qos) {
        this.reply_qos = reply_qos;
    }

    public long getExp_time() {
        return exp_time;
    }

    public void setExp_time(long exp_time) {
        this.exp_time = exp_time;
    }
	
    /**
     * indicates which of the values are set
     * @HasGetter
     * @HasSetter
     */
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
