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
    
    @UbfField(bfldid=test.T_SHORT_2_FLD, ubfmin=0, ojbmin=0)
    String [] tstring;

}
