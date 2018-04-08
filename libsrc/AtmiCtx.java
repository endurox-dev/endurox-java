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
    */
   private static native long tpnewctxt();
   
   
   /**
    * Allocate new ATMI context 
    * @return
    */
   public static AtmiCtx newAtmiCtx() {
	   
/*
	   AtmiCtx ret = new AtmiCtx();
	   
	   if (0==(ret.ctx = AtmiCtx.tpnewctxt())) {
		   //Throw some exception
		   
	   }
	   
	   return ret;
*/
        return null;
	   
   }
   
   // Test Driver
   public static void main(String[] args) {
      //System.out.printf("%d", tpnewctxt());
   }
}

