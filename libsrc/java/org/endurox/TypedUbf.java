package org.endurox;

public class TypedUbf extends TypedBuffer {
	
   /**
    * Initialise UBF Object
    * @param ctx[in] ATMI Context allocated this method
    * @param doFinalize[in] Should
    * @param cPtr[in] C pointer to allocated block
    * @param len[in] Conditional buffer length
    */
   public TypedUbf(AtmiCtx ctx, boolean doFinalize, long cPtr, long len) {
        super(ctx, doFinalize, cPtr, len);
   }
}

