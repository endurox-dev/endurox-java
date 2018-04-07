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
    * Allocate new ATMI Context
    * ptr to ATMI context
    */
   private static native long tpnewctxt();
   
   /**
    * Allocate new ATMI context 
    * @return
    */
   public static AtmiCtx newAtmiCtx() {
	   
	   AtmiCtx ret = new AtmiCtx();
			   
	   if (0==(ret.ctx = AtmiCtx.tpnewctxt())) {
		   //Throw some exception
		   
	   }
	   
	   return ret;
	   
   }
   
   // Test Driver
   public static void main(String[] args) {
      System.out.printf("%d", tpnewctxt());
   }
}

