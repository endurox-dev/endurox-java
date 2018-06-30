package org.endurox;

/**
 * Typed JSON ATMI buffer
 */
public class TypedJson extends TypedBuffer {
	
   /**
    * Initialize JSON buffer
    * @param ctx[in] ATMI Context allocated this method
    * @param doFinalize[in] Should
    * @param cPtr[in] C pointer to allocated block
    * @param len[in] Conditional buffer length
    */
   public TypedJson(AtmiCtx ctx, boolean doFinalize, long cPtr, long len) {
        super(ctx, doFinalize, cPtr, len);
   }
}

