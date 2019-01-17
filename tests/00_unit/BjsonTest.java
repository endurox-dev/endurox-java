import org.junit.Test;
import static org.junit.Assert.*;
import org.endurox.*;

/**
 * Buffer field delete tests
 */
public class BjsonTest {
             
    /**
     * Delete single field test
     */
    @Test
    public void testTpUBFToJSON() {
        
        for (int i=0; i<100; i++)
        {
            AtmiCtx ctx = new AtmiCtx();
            assertNotEquals(ctx.getCtx(), 0x0);

            TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
            assertNotEquals(ub, null);
            
            /* Load some test data in UBF */
            ButilTest.loadTestData1(ub);
            
            ub.Bchg(test.T_CHAR_FLD, 0, 'A');
            ub.Bchg(test.T_CHAR_FLD, 1, 'B');
            
            ub.Bchg(test.T_CHAR_2_FLD, 0, 'C');
            ub.Bchg(test.T_CHAR_2_FLD, 1, 'D');
        
            /* generate some json */
            String json = ub.TpUBFToJSON();
                    
            ctx.tplog(AtmiConstants.LOG_DEBUG, false, "Got json: %s", json);
            
            assertEquals("{\"T_SHORT_FLD\":[455,-5],\"T_SHORT_2_FLD\":[455,-5],\""
                    + "T_LONG_FLD\":[111455,-2225],\"T_LONG_2_FLD\":[111455,-2225],\""
                    + "T_CHAR_FLD\":[\"A\",\"B\"],\"T_CHAR_2_FLD\":[\"C\",\"D\"],\""
                    + "T_FLOAT_FLD\":[33.110001,-500.630005],\"T_FLOAT_2_FLD\""
                    + ":[33.110001,-500.630005],\"T_DOUBLE_FLD\":[1133.110000,-22500.630000],\""
                    + "T_DOUBLE_2_FLD\":[1133.110000,-22500.630000],\"T_STRING_FLD\":[\""
                    + "HELLO\",\"WORLD\",\"OK\"],\"T_CARRAY_FLD\":[\"AAECAw==\",\""
                    + "AAQFBgc=\"],\"T_CARRAY_2_FLD\":[\"AAECAw==\",\"AAQFBgc=\"]}", json);
            
            ctx.cleanup();
            ub.cleanup();
        }
    }    
}
