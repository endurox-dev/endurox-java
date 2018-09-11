import java.io.BufferedReader;
import java.io.IOException;
import java.io.Reader;
import java.nio.CharBuffer;
import static org.junit.Assert.*;
import org.junit.Test;
import org.endurox.*;
import java.util.Locale;

/**
 * Utility method tests
 */
public class ButilTest {
    
    
    /**
     * Have some common test data loader
     * @param ub UBF buffer to load data. Assumes there is enough space
     */
    public static void loadTestData1(TypedUbf ub) {
        
        /* shorts */
        ub.Bchg(test.T_SHORT_FLD, 0, 455);
        ub.Bchg(test.T_SHORT_FLD, 1, -5);
        
        ub.Bchg(test.T_LONG_FLD, 0, 111455);
        ub.Bchg(test.T_LONG_FLD, 1, -2225);
        
        ub.Bchg(test.T_CHAR_FLD, 0, 124);
        ub.Bchg(test.T_CHAR_FLD, 1, 253);
        
        ub.Bchg(test.T_FLOAT_FLD, 0, 33.11);
        ub.Bchg(test.T_FLOAT_FLD, 1, -500.63);
        
        ub.Bchg(test.T_DOUBLE_FLD, 0, 1133.11);
        ub.Bchg(test.T_DOUBLE_FLD, 1, -22500.63);
        
        ub.Bchg(test.T_STRING_FLD, 0, "HELLO");
        ub.Bchg(test.T_STRING_FLD, 1, "WORLD");
        ub.Bchg(test.T_STRING_FLD, 2, "OK");
        
        ub.Bchg(test.T_CARRAY_FLD, 0, new byte[]{0,1,2,3});
        ub.Bchg(test.T_CARRAY_FLD, 1, new byte[]{0,4,5,6,7});
    }
    
    /**
     * Validate test data produced by loadTestData1
     * @param ub UBF buffer with data
     */
    public static void validateTestData(TypedUbf ub) {
        
        assertEquals(455, ub.BgetShort(test.T_SHORT_FLD, 0));
        assertEquals(-5, ub.BgetShort(test.T_SHORT_FLD, 1));
        assertEquals(false, ub.Bpres(test.T_SHORT_FLD, 2));
        
        assertEquals(111455, ub.BgetLong(test.T_LONG_FLD, 0));
        assertEquals(-2225, ub.BgetLong(test.T_LONG_FLD, 1));
        assertEquals(false, ub.Bpres(test.T_LONG_FLD, 2));
        
        assertEquals(124, ub.BgetByte(test.T_CHAR_FLD, 0));
        assertEquals((byte)253, ub.BgetByte(test.T_CHAR_FLD, 1));
        assertEquals(false, ub.Bpres(test.T_CHAR_FLD, 2));
        
        assertEquals(33.11, ub.BgetFloat(test.T_FLOAT_FLD, 0), 0.01);
        assertEquals(-500.63, ub.BgetFloat(test.T_FLOAT_FLD, 1), 0.01);
        assertEquals(false, ub.Bpres(test.T_FLOAT_FLD, 2));
        
        assertEquals(1133.11, ub.BgetDouble(test.T_DOUBLE_FLD, 0), 0.01);
        assertEquals(-22500.63, ub.BgetDouble(test.T_DOUBLE_FLD, 1), 0.01);
        assertEquals(false, ub.Bpres(test.T_DOUBLE_FLD, 2));
        
        assertEquals("HELLO", ub.BgetString(test.T_STRING_FLD, 0));
        assertEquals("WORLD", ub.BgetString(test.T_STRING_FLD, 1));
        assertEquals("OK", ub.BgetString(test.T_STRING_FLD,2));
        assertEquals(false, ub.Bpres(test.T_STRING_FLD, 3));
        
        assertArrayEquals(new byte[]{0,1,2,3}, ub.BgetByteArr(test.T_CARRAY_FLD, 0));
        assertArrayEquals(new byte[]{0,4,5,6,7}, ub.BgetByteArr(test.T_CARRAY_FLD, 1));
        assertEquals(false, ub.Bpres(test.T_CARRAY_FLD, 2));        
    }
    
    /**
     * Test is buffer UBF?
     */
    @Test
    public void testBisubf() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(null, ub);
        assertEquals(true, ub.Bisubf());
    }
    
    /**
     * Test field length
     */
    @Test
    public void testBlen() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(null, ub);
        
        ub.Badd(test.T_STRING_FLD, "HELLO WORLD");
        ub.Badd(test.T_STRING_FLD, "HELLO WORLD1");
        assertEquals(13, ub.Blen(test.T_STRING_FLD, 1));
    }
    
    /**
     * Test Bnext() method call
     */
    @Test
    public void testBnext() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(null, ub);
        
        /* load some test data.. */
        loadTestData1(ub);
        
        /* Now loop over */
        boolean first = true;
        BNextResult res = null;
        
        res = ub.Bnext(first);
        first = false;
        
        assertNotEquals(null, res);
        assertEquals(test.T_SHORT_FLD, res.bfldid);
        assertEquals(0, res.occ);
        /* two bytes ok... */
        assertEquals(2, res.len);
        
        res = ub.Bnext(first);
        assertNotEquals(null, res);
        assertEquals(test.T_SHORT_FLD, res.bfldid);
        assertEquals(1, res.occ);
        /* two bytes ok... */
        assertEquals(2, res.len);
        
        res = ub.Bnext(first);
        assertNotEquals(null, res);
        assertEquals(test.T_LONG_FLD, res.bfldid);
        assertEquals(0, res.occ);
        
        res = ub.Bnext(first);
        assertNotEquals(null, res);
        assertEquals(test.T_LONG_FLD, res.bfldid);
        
        /* not measuring length due to 32bit/64bit platform differences... */
        assertEquals(1, res.occ);
        
        res = ub.Bnext(first);
        assertNotEquals(null, res);
        assertEquals(test.T_CHAR_FLD, res.bfldid);
        assertEquals(0, res.occ);
        assertEquals(1, res.len);
        
        
        res = ub.Bnext(first);
        assertNotEquals(null, res);
        assertEquals(test.T_CHAR_FLD, res.bfldid);
        assertEquals(1, res.occ);
        assertEquals(1, res.len);
        
        res = ub.Bnext(first);
        assertNotEquals(null, res);
        assertEquals(test.T_FLOAT_FLD, res.bfldid);
        assertEquals(0, res.occ);
        
        res = ub.Bnext(first);
        assertNotEquals(null, res);
        assertEquals(test.T_FLOAT_FLD, res.bfldid);
        assertEquals(1, res.occ);
        
        res = ub.Bnext(first);
        assertNotEquals(null, res);
        assertEquals(test.T_DOUBLE_FLD, res.bfldid);
        assertEquals(0, res.occ);
        
        res = ub.Bnext(first);
        assertNotEquals(null, res);
        assertEquals(test.T_DOUBLE_FLD, res.bfldid);
        assertEquals(1, res.occ);
        
        /* these includes EOS.. */
        res = ub.Bnext(first);
        assertNotEquals(null, res);
        assertEquals(test.T_STRING_FLD, res.bfldid);
        assertEquals(0, res.occ);
        assertEquals(6, res.len);
        
        res = ub.Bnext(first);
        assertNotEquals(null, res);
        assertEquals(test.T_STRING_FLD, res.bfldid);
        assertEquals(1, res.occ);
        assertEquals(6, res.len);
        
        res = ub.Bnext(first);
        assertNotEquals(null, res);
        assertEquals(test.T_STRING_FLD, res.bfldid);
        assertEquals(2, res.occ);
        assertEquals(3, res.len);
        
        res = ub.Bnext(first);
        assertNotEquals(null, res);
        assertEquals(test.T_CARRAY_FLD, res.bfldid);
        assertEquals(0, res.occ);
        assertEquals(4, res.len);
        
        res = ub.Bnext(first);
        assertNotEquals(null, res);
        assertEquals(test.T_CARRAY_FLD, res.bfldid);
        assertEquals(1, res.occ);
        assertEquals(5, res.len);
        
        /* we are done! */
        res = ub.Bnext(first);
        assertEquals(null, res);
        
    }
    
    /**
     * Test Boccur
     */
    @Test
    public void testBoccur() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(null, ub);
        
        /* load some test data.. */
        loadTestData1(ub);
        
        assertEquals(ub.Boccur(test.T_STRING_FLD), 3);
        assertEquals(ub.Boccur(test.T_SHORT_FLD), 2);
        
    }
    
    /**
     * Test buffer sizes
     */
    @Test
    public void testBsize() {
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(null, ub);
        
        /* load some test data.. */
        loadTestData1(ub);
        
        assertTrue(ub.Bsizeof()>=1024);
        
        assertTrue(ub.Bused()>=16);
        assertTrue(ub.Bunused()>=16);
        
        assertEquals(ub.Bsizeof(), ub.Bused()+ub.Bunused());
        
    }
    
    
}
  
