package org.endurox;

/**
 * Service call information
 */
public class TpSvcInfo {    
    
    /** Service name */
    String  name;
    
    /** ATMI data buffer */
    TypedBuffer data;
    
    /** Call flags */
    long    flags;
    
    /** Call descriptor */
    int     cd;
    
    /** RFU */
    long    appkey;
    
    /** Client ID */
    ClientId cltId;
    
    /** Invoked function name */
    String  fname;

    /** @return Called service name */
    public String getName() {
        return name;
    }

    /** @return Call data */
    public TypedBuffer getData() {
        return data;
    }

    /** @return call flags */
    public long getFlags() {
        return flags;
    }

    /** @return call descriptor */
    public int getCd() {
        return cd;
    }

    /**@return RFU */
    public long getAppkey() {
        return appkey;
    }

    /** @return Get client ID */
    public ClientId getCltId() {
        return cltId;
    }

    /** @return Get function invoked name  */
    public String getFname() {
        return fname;
    }
    
    /**
     * Create Service call info object
     * @param name service name
     * @param data call data
     * @param flags call flags
     * @param cd    Call descriptor
     * @param appkey    RFU
     * @param cltId client id object
     * @param fname function name invoked
     */
    public TpSvcInfo(String name, TypedBuffer data, long flags, int cd, long appkey, 
            ClientId cltId, String fname) {
        this.name = name;
        this.data = data;
        this.flags = flags;
        this.cd = cd;
        this.appkey = appkey;
        this.cltId = cltId;
        this.fname = fname;
    }
}
