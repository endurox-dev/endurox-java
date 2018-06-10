package org.endurox;

public class AtmiBuf {
	
    /**
     * Pointer to C ATMI Context object
     */
    private AtmiCtx ctx;
    private boolean doFinalize;
    
    /* Allow access from package: */
    long len;
    long cPtr;

    /**
     * Free up the given buffer
     * @param[in] cPtr C pointer to buffer
     */
    private native void tpfree (long cPtr);

    /**
     * Initialize ATMI Object
     * @param ctx[in] ATMI Context allocated this method
     * @param doFinalize[in] Should
     * @param cPtr[in] C pointer to allocated block
     * @param len[in] Conditional buffer length
     */
    public AtmiBuf(AtmiCtx ctx, boolean doFinalize, long cPtr, long len) {
         this.ctx = ctx;
         this.doFinalize = doFinalize;
         this.cPtr = cPtr;
         this.len = len;

         System.out.format("AtmiBuf allocated.\n");

    }

    /**
     * Finish of this ATMI buffer
     */
    @Override
    protected void finalize() {
         //Call free from context?
         //Or call directly the c? I guess directly as context might be already
         //invalid

         System.out.format("HELLO FINAL\n");

         if (doFinalize) {
             tpfree(cPtr);
         }
    }

    /**
     * Set the finalize flag
     */
    public void setDoFinalize(boolean b) {
        doFinalize = b;
    }

   static {
      System.loadLibrary("exjava"); // Load native library at runtime
                           // hello.dll (Windows) or libenduroxjava.so (Unixes)
   }
}

