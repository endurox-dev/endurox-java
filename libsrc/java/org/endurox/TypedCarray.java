package org.endurox;

public class TypedCarray extends TypedBuffer {
	
   /**
    * Initialise Carray buffer
    * @param ctx[in] ATMI Context allocated this method
    * @param doFinalize[in] Should the buffer be deallocated automaitcally
    * @param cPtr[in] C pointer to allocated block
    * @param len[in] Conditional buffer length
    */
   public TypedCarray(AtmiCtx ctx, boolean doFinalize, long cPtr, long len) {
        super(ctx, doFinalize, cPtr, len);
   }
}

