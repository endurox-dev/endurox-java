package org.endurox;

public class TypedUBF extends TypedBuffer {
	
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
   
       /* TODO: Add Badd func... */
    
    /**
     * Add short value to UBF buffer
     * @param bfldid compiled field id
     * @param s short value
     */
    public native void Badd(int bfldid, short s);
    
    /**
     * Added Long value to UBF buffer
     * @param bfldid field id
     * @param l long value
     * @throws BALIGNERR, BNOTFLD, BNOSPACE
     */
    public native void Badd(int bfldid, long l);
    
    public native void Badd(int bfldid, char c);
    
    public native void Badd(int bfldid, float f);
    
    public native void Badd(int bfldid, double d);
    
    public native void Badd(int bfldid, String s);
    
    public native void Badd(int bfldid, byte []b);
    
    /**
     * Print the UBF buffer to STDOUT
     * @throws AtmiBALIGNERRException Corrupted buffer or pointing to not aligned memory area.
     * @throws AtmiBNOTFLDException Buffer not fielded, not correctly allocated or corrupted.
     */
    public native void Bprint();

}

