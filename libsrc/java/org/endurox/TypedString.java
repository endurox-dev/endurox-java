package org.endurox;

public class TypedString extends TypedBuffer {
	
   /**
    * Initialise String buffer
    * @param ctx[in] ATMI Context allocated this method
    * @param doFinalize[in] Should
    * @param cPtr[in] C pointer to allocated block
    * @param len[in] Conditional buffer length
    */
   public TypedString(AtmiCtx ctx, boolean doFinalize, long cPtr, long len) {
        super(ctx, doFinalize, cPtr, len);
   }
}

