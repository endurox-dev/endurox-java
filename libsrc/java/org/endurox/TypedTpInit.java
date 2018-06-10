package org.endurox;

/**
 * Typed INIT buffer
 */
public class TypedTpInit extends AtmiBuf {
	
   /**
    * Initialize TPINIT buffer
    * @param ctx[in] ATMI Context allocated this method
    * @param doFinalize[in] Should
    * @param cPtr[in] C pointer to allocated block
    * @param len[in] Conditional buffer length
    */
   public TypedTpInit(AtmiCtx ctx, boolean doFinalize, long cPtr, long len) {
        super(ctx, doFinalize, cPtr, len);
   }
}
