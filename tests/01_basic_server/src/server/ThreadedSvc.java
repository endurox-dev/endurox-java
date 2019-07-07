import org.endurox.*;

public class ThreadedSvc implements Service {

    
    class ProcThread implements Runnable {
        
        private Thread t;
        TypedBuffer buf;
        AtmiCtx ctx = new AtmiCtx();
        long svctxPtr;
        
        ProcThread(long ptr, TypedBuffer buf) {
            this.buf = buf;
            this.svctxPtr = ptr;
        }
        
        /**
         * Run the threaded test...
         */
        public void run() {
            
            ctx.tplogDebug("Thread service got call");
            
            /* we are now owners of the buffer, otherwise we will try to lock
             * other context
             */
            buf.setAtmiCtx(ctx);
            
            /* start XATMI ... */
            ctx.tpinit(null);
            
            /* Restore the context */
            ctx.tpsrvsetctxdata(svctxPtr, AtmiConst.SYS_SRV_THREAD);
            ctx.tpsrvfreectxdata(svctxPtr);
        
            /* we need to swap the ATMI Context for the buffer...! */
            TypedUbf ub = (TypedUbf)buf;
            
            /* set new size */
            ub.tprealloc(1024);
            ctx.tplogDebug("realloc OK...");

            /* Add new value */
            ub.Badd(test.T_STRING_FLD, "UBF OK 2");

            ctx.tplogDebug("return...");
            ctx.tpreturn(AtmiConst.TPSUCCESS, 0, buf, 0);
            
            /* terminate the xatmi session */
            ctx.tpterm();
            ctx.cleanup();
            
        }
        
        /**
         * Run off the thread
         */
        public void start () {
            
            if (t == null) {
                t = new Thread (this);
                t.start ();
            }
        }
        
    }
    
    /**
     * Multi threaded service, will copy the work to new thread...
     * @param ctx
     * @param svcinfo 
     */
    public void tpService(AtmiCtx ctx, TpSvcInfo svcinfo) {
        
        long ptr = ctx.tpsrvgetctxdata();
        
        /* the data shall be copied to thread too: */
        TypedUbf buf = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        buf.Bcpy((TypedUbf)svcinfo.getData());
        
        /* flip over the task to thread */
        ProcThread thread = new ProcThread(ptr, buf);
        
        thread.start();
        
        /* continue servicing */
        ctx.tpcontinue();
    }

}
