/**
 * @brief UBF buffer type
 *
 * @class TypedUbf
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

import java.io.BufferedReader;
import org.endurox.exceptions.AtmiTPENOENTException;
import org.endurox.exceptions.UbfBALIGNERRException;
import org.endurox.exceptions.UbfBBADFLDException;
import org.endurox.exceptions.UbfBNOTFLDException;
import org.endurox.exceptions.UbfBNOTPRESException;

public class TypedUbf extends TypedBuffer {
	
    static {
       System.loadLibrary("exjava"); // Load native library at runtime
    }
    
    /**
     * Initialize UBF Object
     * @param ctx[in] ATMI Context allocated this method
     * @param doFinalize[in] Should
     * @param cPtr[in] C pointer to allocated block
     * @param len[in] Conditional buffer length
     */
    public TypedUbf(AtmiCtx ctx, boolean doFinalize, long cPtr, long len) {
         super(ctx, doFinalize, cPtr, len);
    }
    
    /**
     * Add field to UBF buffer. Group of methods for different data types.
     * @defgroup Badd function calls
     * @param bfldid compiled field id
     * @throw UbfBALIGNERRException Invalid Buffer
     * @throw UbfBNOTFLDException Invalid Buffer
     * @throw UbfBNOSPACEException No space in buffer
     * @{
     */
    
    /**
     * Add short value to UBF buffer
     * @param bfldid compiled field id
     * @param s short value
     */
    public native void Badd(int bfldid, short s);

    /**
     * Add long value to UBF
     * @param bfldid compiled field id
     * @param l long value
     */
    public native void Badd(int bfldid, long l);
    
    /**
     * Add byte (ANSI Char) to UBF
     * @param bfldid compiled field id
     * @param c ANSI char value / Java byte
     */
    public native void Badd(int bfldid, byte c);
    
    /**
     * Set float value to buffer
     * @param bfldid compiled field id
     * @param f float value
     */
    public native void Badd(int bfldid, float f);
    
    /**
     * Set Double value to buffer
     * @param bfldid field id
     * @param d  double value
     */
    public native void Badd(int bfldid, double d);
    
    /**
     * Add string to UBF buffer
     * @param bfldid field id
     * @param s String value
     */
    public native void Badd(int bfldid, String s);
    
    /**
     * Add byte array to UBF buffer
     * @param bfldid field id
     * @param b byte array
     */
    public native void Badd(int bfldid, byte []b);
    
    /** @} */ // end of Badd
    
    /**
     * Print the UBF buffer to STDOUT
     * @throw AtmiBALIGNERRException Corrupted buffer or 
     *  pointing to not aligned memory area.
     * @throw AtmiBNOTFLDException Buffer not fielded, not 
     *  correctly allocated or corrupted.
     */
    public native void Bprint();
    
    
    /**
     * Get field value from UBF buffer. The values are casted according function
     * return types.
     * @defgroup Bget function calls
     * @param bfldid compiled field id
     * @param occ field occurrence
     * @return casted data type
     * @throw UbfBALIGNERRException Invalid Buffer
     * @throw UbfBNOTFLDException Invalid Buffer
     * @throw UbfBNOSPACEException No space in buffer
     * @{
     */
    public native short BgetShort(int bfldid, int occ);
    public native long BgetLong(int bfldid, int occ);
    public native byte BgetByte(int bfldid, int occ);
    public native float BgetFloat(int bfldid, int occ);
    public native double BgetDouble(int bfldid, int occ);
    public native String BgetString(int bfldid, int occ);
    public native byte[] BgetByteArr(int bfldid, int occ);
    
    /** @} */ // end of Bget
    
    /**
     * Fast add field to UBF buffer. This function is used when series of the
     *  same field occurrences are added to buffer. This saves a pointer to
     *  last buffer offset where data is finished to add. Thus next add operation
     *  will continue from that position. This greatly increases the buffer
     *  population speed. Between calls, no modifications shall be done to buffer,
     *  otherwise that might results in corrupted UBF buffer.
     *  When adding first field in the batch, then BFldLocInfo must be reset.
     *  Either by creating new object or explicitly calling reset() method
     *  on the object.
     * @defgroup Baddfast function calls
     * @param bfldid compiled field id
     * @throw UbfBALIGNERRException Invalid Buffer
     * @throw UbfBNOTFLDException Invalid Buffer
     * @throw UbfBNOSPACEException No space in buffer
     * @{
     */
    
    /**
     * Add short value to UBF buffer
     * @param bfldid compiled field id
     * @param s short value
     * @param next_fld storage for last field added pointer.
     */
    public native void Baddfast(int bfldid, short s, BFldLocInfo next_fld);

    /**
     * Add long value to UBF
     * @param bfldid compiled field id
     * @param l long value
     * @param next_fld storage for last field added pointer.
     */
    public native void Baddfast(int bfldid, long l, BFldLocInfo next_fld);
    
    /**
     * Add byte (ANSI Char) to UBF
     * @param bfldid compiled field id
     * @param c ANSI char value / Java byte
     * @param next_fld storage for last field added pointer.
     */
    public native void Baddfast(int bfldid, byte c, BFldLocInfo next_fld);
    
    /**
     * Set float value to buffer
     * @param bfldid compiled field id
     * @param f float value
     * @param next_fld storage for last field added pointer.
     */
    public native void Baddfast(int bfldid, float f, BFldLocInfo next_fld);
    
    /**
     * Set Double value to buffer
     * @param bfldid field id
     * @param d  double value
     * @param next_fld storage for last field added pointer.
     */
    public native void Baddfast(int bfldid, double d, BFldLocInfo next_fld);
    
    /**
     * Add string to UBF buffer
     * @param bfldid field id
     * @param s String value
     * @param next_fld storage for last field added pointer.
     */
    public native void Baddfast(int bfldid, String s, BFldLocInfo next_fld);
    
    /**
     * Add byte array to UBF buffer
     * @param bfldid field id
     * @param b byte array
     * @param next_fld storage for last field added pointer.
     */
    public native void Baddfast(int bfldid, byte []b, BFldLocInfo next_fld);
    
    /** @} */ // end of Badd
    
    /**
     * Add/Update (change) field value at given occurrence
     * @defgroup Bchg function calls
     * @param bfldid compiled field id
     * @param occ field occurrence to change
     * @throw UbfBALIGNERRException Invalid Buffer
     * @throw UbfBNOTFLDException Invalid Buffer
     * @throw UbfBNOSPACEException No space in buffer
     * @throw UbfBBADFLDException No space in buffer
     * @{
     */
    
    /**
     * Change short value
     * @param s short value
     */
    public native void Bchg(int bfldid, int occ, short s);

    /**
     * Change long value
     * @param l long value
     */
    public native void Bchg(int bfldid, int occ, long l);
    
    /**
     * Change byte (ANSI Char) to UBF
     * @param c ANSI char value / Java byte
     */
    public native void Bchg(int bfldid, int occ, byte c);
    
    /**
     * Change float value
     * @param f float value
     */
    public native void Bchg(int bfldid, int occ, float f);
    
    /**
     * Change Double value
     * @param d  double value
     */
    public native void Bchg(int bfldid, int occ, double d);
    
    /**
     * Change string
     * @param s String value
     */
    public native void Bchg(int bfldid, int occ, String s);
    
    /**
     * Change byte
     * @param b byte array
     */
    public native void Bchg(int bfldid, int occ, byte []b);
    
    /** @} */ // end of Bchg
    
    /**
     * @defgroup Bboolubf UBF boolean operations 
     */
    
    /**
     * Dispatch callback function. The callback object must be registered
     * in the ubfcbMap. If not there, then AtmiTPENOENTException exception
     * is thrown
     * @param funcname function name received from expression evaluator.
     * @throws AtmiTPENOENTException if function is not registered in the system
     */
    long boolcbfDispatch (String funcname) {
        /* This will use singleton functions registered in the ATMI Context */
        Bboolcbf cbf = ctx.BoolgetcbfObj(funcname);
        
        if (null==cbf) {
            throw new AtmiTPENOENTException(String.format("Function not found: "+
                    "[%s] in java space", 
                    funcname));
        }
        return cbf.bboolCallBack(ctx, this, funcname);
    }

    /**
     * Group of methods related to executing the boolean expression on UBF buffer.
     * In case if compiled boolean expression is used, then \ref AtmiCtx.Bboolco()
     * shall be used.
     * @throws UbfBNOTFLDException p_ub is not fielded buffer or argument NULL.
     * @throws UbfBEINVALException tree parameter is NULL.
     * @throws UbfBALIGNERRException Corrupted buffer or pointing to not aligned memory area.
     * @throws UbfBNOTFLDException Buffer not fielded, not correctly allocated or corrupted.
     * @throws UbfBBADFLDException Invalid field id passed.
     * @throws UbfBNOTPRESException Field not present.
     * @throws UbfFMALLOCException Malloc failed.
     * @throws UbfBSYNTAXException Synax error in script.
     * @throws UbfBBADNAMEException Bad field name specified.
     * @defgroup Bboolev boolean expression handling routines
     * @{
     */
    /**
     * Evaluate boolean expression by given expression tree. 
     * For more information see Bboolev(3) manpage.
     * @param tree compiled boolean expression
     * @return true or false
     * @{
     */
    public native boolean Bboolev(BExprTree tree);

    /**
     * Evaluate the boolean expression immediately
     * and the release the compiled expression. 
     * For more information see Bboolco(3) and Bboolev(3) manpages.
     * @param expr boolean expression string
     * @return true or false
     */
    public native boolean Bqboolev(String expr);

    /**
     * Evaluate compiled expression and return the result as a float value.
     * For more information see Bfloatev(3) manpage
     * @param tree compiled boolean expression
     * @return evaluated floating point value
     */
    public native double Bfloatev(BExprTree tree);

    /** @} */ // end of Bboolubf
    
    /**
     * Group of methods related with data erase from the UBF buffer.
     * @defgroup Bdelete field erase methods from UBF buffer
     * @{
     */
    /**
     * Delete field occurrence from UBF buffer.
     * For more information see Bdel(3) manpage.
     * @param bfldid compiled field id
     * @param occ occurrence to erase
     * UbfBALIGNERRException Corrupted buffer or pointing to not aligned memory area.
     * UbfBNOTFLDException Buffer not fielded, not correctly allocated or corrupted.
     * UbfBBADFLDException Invalid field id passed.
     * UbfBNOTPRESException Field not present.
     */
    public native void Bdel(int bfldid, int occ);
    
    /**
     * Delete all occurrences from the UBF buffer.
     * For more information see Bdelall(3) manpage.
     * @param bfldid compiled field id
     * @throws UbfBALIGNERRException Corrupted buffer or pointing to 
     *      not aligned memory area.
     * @throws UbfBNOTFLDException Buffer not fielded, not correctly 
     *      allocated or corrupted.
     * @throws UbfBBADFLDException Invalid field id passed.

     * @throws UbfBNOTPRESException Field not present thus not deleted.
     */
    public native void Bdelall(int bfldid);
    
    /**
     * Delete list of fields from UBF buffer
     * For more information see Bdelete(3) manpage.
     * @param bfldid array of fields which needs to be deleted. Note that as Java
     *  have array length information, no BBADFLDID is required at the end of
     *  the array.
     * @throws UbfBALIGNERRException Corrupted buffer or pointing to 
     *      not aligned memory area.
     * @throws UbfBNOTFLDException Buffer not fielded, not correctly 
     *      allocated or corrupted.
     */
    public native void Bdelete(int [] bfldid);
    
    /**
     * Test for field presence in UBF buffer.
     * For more information see Bpres(3) manpage.
     * @param bfldid compiled UBF buffer id
     * @param occ field occurrence (0 based).
     * @return true - field present, false - field not present or error occurred
     */
    public native boolean Bpres(int bfldid, int occ);
    
    
    /**
     * Restore UBF buffer from printed text buffer in
     * the stream. For data format description
     * see Bextread(3) manpage. Basically format is:
     * <FIELDNAME>\t<VALUE>\n
     * @param reader input stream interface. The stream will be read line by line
     * @thows UbfBALIGNERRException Corrupted buffer or pointing 
     *  to not aligned memory area.
     * @thows UbfBNOTFLDException Buffer not fielded, not correctly 
     *  allocated or corrupted.
     * @thows UbfBSYNTAXException Missing tab between field id 
     *  or missing newline.
     * @thows UbfBBADNAMEException Field not found in field table.
     */
    public native void Bextread(BufferedReader reader);
    
    /**
     * Test UBF buffer to see does underlying memory corresponds to the UBF format.
      For more information see Bisubf(3) manpage.
     * @return true - buffer is UBF, false - buffer is not UBF
     */
    public native boolean Bisubf();
    
}

/* vim: set ts=4 sw=4 et smartindent: */
