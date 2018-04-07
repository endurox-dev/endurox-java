public class AtmiCtx {
   static {
      System.loadLibrary("exjava"); // Load native library at runtime
                                   // hello.dll (Windows) or libenduroxjava.so (Unixes)
   }
 
   /**
    * Allocate new ATMI Context
    * ptr to ATMI context
    */
   private static native long tpnewctxt();
 
   // Test Driver
   public static void main(String[] args) {
        
        
      System.out.printf("%d", tpnewctxt());
   }
}

