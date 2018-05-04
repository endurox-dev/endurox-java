package org.endurox;

/**
 * Service call information
 */
public class TpSvcInfo {    
    
    /**
     * Service name
     */
    String  name;
    
    /**
     * ATMI data buffer
     */
    AtmiBuf data;
    
    /**
     * Call flags
     */
    long    flags;
    
    /**
     * Call descriptor
     */
    int     cd;
    
    /**
     * RFU
     */
    long    appkey;
    
    /**
     * Client ID
     */
    ClientID cltid;
    
    /**
     * Invoked function name
     */
    String  fname;
    
}
