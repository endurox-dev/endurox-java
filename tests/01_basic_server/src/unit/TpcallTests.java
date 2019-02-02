import org.junit.Test;
import static org.junit.Assert.*;
import org.endurox.*;
import org.endurox.exceptions.UbfBNOTPRESException;

/**
 * Buffer field delete tests
 */
public class TpcallTests {
             
    /**
     * Perform tpcall test
     */
    @Test
    public void tpcallTest() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);

        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);

        /**
         * TODO: Have long term test for memory management.
         * ideally we would time terminated tests, for example 5 min...?
         * thus we need a stop watch construction to have in java..
         */
        for (int i=0; i<100000000; i++)
        {
            
            try {
                ub.Bdel(test.T_STRING_2_FLD, 0);
            } 
            catch (UbfBNOTPRESException e)
            {
                // ignore.. 
            }
            
            String reqData = String.format("loop %d", i);
            ub.Bchg(test.T_STRING_FLD, 0, String.format("loop %d", i));
            
            ub = (TypedUbf)ctx.tpcall("ECHOSVC", ub, 0);
            
            String rspData = ub.BgetString(test.T_STRING_2_FLD, 0);
            assertEquals(reqData, rspData);
        }
        ub.cleanup();
        ctx.cleanup();
    }
    
    //TODO: Test with NULL buffer call....
    
    //TODO: Test buffer with other service error, for example noent...
    
    //TODO: Test server call with buffer translate...
    
    //TODO: Test server call with buffer translate and service error...
}
