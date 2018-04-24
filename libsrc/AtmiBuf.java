package org.endurox;

public class AtmiBuf {
	
  /**
   * Pointer to C ATMI Context object
   */
   private Atmictx ctx;
   private bool haveFinalizer;
   private long len;
   private long cPtr;

   @Override
   public void finalize() {
        //Call free from context?
        //Or call directly the c? I guess directly as context might be already
        //invalid
   }

   static {
      System.loadLibrary("exjava"); // Load native library at runtime
                           // hello.dll (Windows) or libenduroxjava.so (Unixes)
   }

}

