import org.endurox.*;

/**
 * More complicated mandatory/optional testing class
 */
public class BMarshalClassMandOpt2 {
             
    /* standard types: 
     * well for this one we shall test what will happen if we try process
     * single occ, it should be accepted ok (no exception), even we have min 2
     */
    @UbfField(bfldid=test.T_SHORT_FLD, ubfmin=2, ojbmin=2)
    short [] tshort;
    
    @UbfField(bfldid=test.T_SHORT_2_FLD, ubfmin=1, ojbmin=1)
    Short [] tshort2;
    
    @UbfField(bfldid=test.T_STRING_FLD, ubfmin=0, ojbmin=0)
    String [] tstring;
    
    /* test also single fields, will use boxed types to allow nulls */
    @UbfField(bfldid=test.T_LONG_FLD, ubfmin=1, ojbmin=1)
    Long tlong;
    
    @UbfField(bfldid=test.T_STRING_FLD, ubfmin=1, ojbmin=1)
    String tstring2;
    

    public short[] getTshort() {
        return tshort;
    }

    public void setTshort(short[] tshort) {
        this.tshort = tshort;
    }

    public Short[] getTshort2() {
        return tshort2;
    }

    public void setTshort2(Short[] tshort2) {
        this.tshort2 = tshort2;
    }

    public String[] getTstring() {
        return tstring;
    }

    public void setTstring(String[] tstring) {
        this.tstring = tstring;
    }
}
