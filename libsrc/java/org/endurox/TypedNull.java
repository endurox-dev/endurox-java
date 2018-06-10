package org.endurox;

/**
 * Typed NULL buffer
 */
public class TypedNull extends AtmiBuf {
	
   /**
    * Initialize TPINIT buffer
    * @param ctx[in] ATMI Context allocated this method
    * @param doFinalize[in] Should
    * @param cPtr[in] C pointer to allocated block
    * @param len[in] Conditional buffer length
    */
   public TypedNull(AtmiCtx ctx, boolean doFinalize, long cPtr, long len) {
        super(ctx, doFinalize, cPtr, len);
   }
}
