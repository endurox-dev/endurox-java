package org.endurox;

public class TypedUBF extends TypedBuffer {
	
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
    public TypedUBF(AtmiCtx ctx, boolean doFinalize, long cPtr, long len) {
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
    public native byte[] BgetByteArray(int bfldid, int occ);
    
    /** @} */ // end of Bget

}

