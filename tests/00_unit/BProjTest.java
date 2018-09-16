import java.io.IOException;
import java.io.OutputStream;
import java.util.concurrent.TimeUnit;
import org.junit.Test;
import static org.junit.Assert.*;
import org.endurox.*;

/**
 * Buffer projection tests
 */
public class BProjTest {
             
    /**
     * Delete single field test
     */
    @Test
    public void testBproj() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);
        
        for (int i=0; i<100; i++)
        {
            ButilTest.loadTestData1(ub);
            
            ub.Bproj(new int[]{test.T_STRING_FLD, test.T_SHORT_FLD});
            
            assertEquals(true, ub.Bpres(test.T_SHORT_FLD, 0));
            assertEquals(true, ub.Bpres(test.T_SHORT_FLD, 1));
            assertEquals(false, ub.Bpres(test.T_SHORT_FLD, 2));
            
            assertEquals(false, ub.Bpres(test.T_LONG_FLD, 0));
            assertEquals(false, ub.Bpres(test.T_CHAR_FLD, 0));
            assertEquals(false, ub.Bpres(test.T_FLOAT_FLD, 0));
            assertEquals(false, ub.Bpres(test.T_DOUBLE_FLD, 0));
            
            assertEquals(true, ub.Bpres(test.T_STRING_FLD, 0));
            assertEquals(true, ub.Bpres(test.T_STRING_FLD, 1));
            assertEquals(true, ub.Bpres(test.T_STRING_FLD, 2));
            assertEquals(false, ub.Bpres(test.T_STRING_FLD, 3));
            
            assertEquals(false, ub.Bpres(test.T_CARRAY_FLD, 0));
            
        }
    }
    
    /**
     * Test for contacting two buffers
     */
    @Test
    public void testBconcat() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        for (int i=0; i<100; i++)
        {
            TypedUbf ub1 = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
            assertNotEquals(ub1, null);

            TypedUbf ub2 = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
            assertNotEquals(ub2, null);

            /* shorts */
            ub1.Bchg(test.T_SHORT_FLD, 0, 455);
            ub2.Bchg(test.T_SHORT_FLD, 0, -5);

            ub1.Bchg(test.T_LONG_FLD, 0, 111455);
            ub2.Bchg(test.T_LONG_FLD, 0, -2225);

            ub1.Bchg(test.T_CHAR_FLD, 0, 124);
            ub2.Bchg(test.T_CHAR_FLD, 0, 253);

            ub1.Bchg(test.T_FLOAT_FLD, 0, 33.11);
            ub2.Bchg(test.T_FLOAT_FLD, 0, -500.63);

            ub1.Bchg(test.T_DOUBLE_FLD, 0, 1133.11);
            ub2.Bchg(test.T_DOUBLE_FLD, 0, -22500.63);

            ub1.Bchg(test.T_STRING_FLD, 0, "HELLO");
            ub2.Bchg(test.T_STRING_FLD, 0, "WORLD");
            ub2.Bchg(test.T_STRING_FLD, 1, "OK");

            ub1.Bchg(test.T_CARRAY_FLD, 0, new byte[]{0,1,2,3});
            ub2.Bchg(test.T_CARRAY_FLD, 0, new byte[]{0,4,5,6,7});

            ub1.Bconcat(ub2);

            ButilTest.validateTestData(ub1);
        }
    }

    /**
     * Test buffer copy
     */
    @Test
    public void testBcpy() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        for (int i=0; i<100; i++)
        {
            TypedUbf ub1 = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
            assertNotEquals(ub1, null);

            TypedUbf ub2 = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
            assertNotEquals(ub2, null);
            
            ButilTest.loadTestData1(ub1);
            
            ub2.Bconcat(ub1);

            ButilTest.validateTestData(ub2);
        }
    }
    
    /**
     * Test buffer projection copy
     */
    @Test
    public void testBprojcpy() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        
        for (int i=0; i<100; i++)
        {
            TypedUbf ub1 = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
            assertNotEquals(ub1, null);

            TypedUbf ub2 = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
            assertNotEquals(ub2, null);
            
            
            ButilTest.loadTestData1(ub1);
            
            ub1.Bchg(test.T_STRING_2_FLD, 0, "A");
            ub1.Bchg(test.T_STRING_2_FLD, 1, "B");
            ub1.Bchg(test.T_STRING_2_FLD, 2, "C");
        
            ub2.Bprojcpy(ub1, new int [] {
                    test.T_SHORT_FLD
                    ,test.T_LONG_FLD
                    ,test.T_CHAR_FLD
                    ,test.T_FLOAT_FLD
                    ,test.T_DOUBLE_FLD
                    ,test.T_STRING_FLD
                    ,test.T_CARRAY_FLD});
            
            /* These parts now should be fine.. */
            ButilTest.validateTestData(ub2);
            //This shall not be copied.
            assertEquals(false, ub2.Bpres(test.T_STRING_2_FLD, 0));
        }
    }
    
    /**
     * Test buffer update
     */
    @Test
    public void testBupdate() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        
        for (int i=0; i<100; i++)
        {
            TypedUbf ub1 = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
            assertNotEquals(ub1, null);

            TypedUbf ub2 = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
            assertNotEquals(ub2, null);
            
            
            ButilTest.loadTestData1(ub1);
            ButilTest.loadTestData1(ub2);
            
            /* set some invalid data in buffer 2 */
            ub2.Bchg(test.T_STRING_FLD, 0, "A");
            ub2.Bchg(test.T_STRING_FLD, 1, "B");
            ub2.Bchg(test.T_STRING_FLD, 2, "C");
            
            /* delete some fields too */
            
            ub2.Bdel(test.T_CARRAY_FLD, 0);
            ub2.Bdel(test.T_CARRAY_FLD, 0);
        
            ub2.Bupdate(ub1);
            
            /* These parts now should be fine.. */
            ButilTest.validateTestData(ub2);
        }
    }
    
    /**
     * Compare buffers
     */
    @Test
    public void testBcmp() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        
        for (int i=0; i<100; i++)
        {
            TypedUbf ub1 = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
            assertNotEquals(ub1, null);

            TypedUbf ub2 = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
            assertNotEquals(ub2, null);
            
            ButilTest.loadTestData1(ub1);
            ButilTest.loadTestData1(ub2);

            assertEquals(0, ub2.Bcmp(ub1));
            
            ub2.Bchg(test.T_STRING_FLD, 2, "C");
                    
            assertNotEquals(0, ub2.Bcmp(ub1));
            
        }
    }
 
    /**
     * Test that buffer is subset of another buffer
     */
    @Test
    public void testBsubset() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        
        for (int i=0; i<100; i++)
        {
            TypedUbf ub1 = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
            assertNotEquals(ub1, null);

            TypedUbf ub2 = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
            assertNotEquals(ub2, null);
            
            ButilTest.loadTestData1(ub1);
            
            //Empty is subset of full
            assertEquals(true, ub1.Bsubset(ub2));
            
            //This also is subset
            ub2.Bchg(test.T_STRING_FLD, 0, "HELLO");
            ub2.Bchg(test.T_STRING_FLD, 1, "WORLD");
            ub2.Bchg(test.T_STRING_FLD, 2, "OK");
                    
            assertEquals(true, ub1.Bsubset(ub2));
            
            //this is not subset
            ub2.Bchg(test.T_STRING_FLD, 0, "HELLO");
            ub2.Bchg(test.T_STRING_FLD, 1, "WORLD");
            ub2.Bchg(test.T_STRING_FLD, 2, "OKX");
                    
            assertEquals(false, ub1.Bsubset(ub2));
            
        }
    }
}
