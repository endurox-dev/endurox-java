package org.endurox;

public class AtmiCtx {
	
    /**
     * Pointer to C ATMI Context object
     */
   private long ctx;
	
   static {
      System.loadLibrary("exjava"); // Load native library at runtime
                           // hello.dll (Windows) or libenduroxjava.so (Unixes)
   }
 
   /**
    * Get ATMI Error 
    * @return Error tuple (code and message)
    */
    private native ErrorTuple getAtmiError();
   
   /**
    * Allocate new ATMI Context
    * ptr to ATMI context
    * @return C pointer to context object
    * @exception AtmiTPESYSTEMException
    */
    private static native long tpnewctxt();

   /**
    * Free up ATMI Context
    * @param ctx ATMI Context
    */
    private static native void tpfreectxt(long ctx);
   
    /**
     * Allocate buffer
     * Thow some exceptions.. too!
     */
    public native void tpAlloc(String btype, String bsubtype, long size);

    /**
     * Allocate new ATMI Context
     */
    public AtmiCtx()  {
        // This thorws TPESYSTEM if failed.
        ctx = tpnewctxt();
    }

    protected void finalize() {
        //Remove ATMI context...
    }
 
   // Test Driver
   public static void main(String[] args) {
    AtmiCtx ctx = new AtmiCtx();

    ErrorTuple err = ctx.getAtmiError();
      System.out.printf("%d: %s\n", err.err, err.msg);
   }
}

