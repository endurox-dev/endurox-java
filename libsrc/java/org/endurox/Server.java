package org.endurox;

/**
 * XATMI Server interface
 */
public interface Server {
    /**
     * Server init call
     * @param argv Enduro/X command line options
     */
    public void tpSvrInit(String [] argv);
    
    /**
     * Server call when shutdown of server process is performed
     */
    public void tpSvrDone();
}


