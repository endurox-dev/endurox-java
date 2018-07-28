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
     * Add short value to UBF buffer
     * @param bfldid compiled field id
     * @param s short value
     * @throw UbfBALIGNERRException Invalid Buffer
     * @trhow UbfBNOTFLDException Invalid Buffer
     * @throw UbfBNOSPACEException No space in buffer
     */
    public native void Badd(int bfldid, short s);
    
    /**
     * Added Long value to UBF buffer.
     * @param bfldid field id
     * @param l long value
     */
    public native void Badd(int bfldid, long l);
    
    public native void Badd(int bfldid, byte c);
    
    public native void Badd(int bfldid, float f);
    
    public native void Badd(int bfldid, double d);
    
    public native void Badd(int bfldid, String s);
    
    public native void Badd(int bfldid, byte []b);
    
    /**
     * Print the UBF buffer to STDOUT
     * @throw AtmiBALIGNERRException Corrupted buffer or 
     *  pointing to not aligned memory area.
     * @throw AtmiBNOTFLDException Buffer not fielded, not 
     *  correctly allocated or corrupted.
     */
    public native void Bprint();
    
    /**
     * Get short value from UBF buffer
     * @param bfldid compiled field id
     * @param occ field occurrence
     * @return value of the field
     * @throw UbfBALIGNERRException Corrupted buffer or pointing to not 
     *  aligned memory area.
     * @throw UbfBNOTFLD Buffer not fielded, not correctly allocated or corrupted.
     * @throw UbfBNOSPACE No space in buf.
     * @throw UbfBBADFLD Invalid field id passed.
     * @throw UbfBNOTPRES Field not present.
     */
    public native short BgetShort(int bfldid, int occ);

}

