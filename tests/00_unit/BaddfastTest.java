import static org.junit.Assert.*;
import org.junit.Test;
import org.endurox.*;
import org.endurox.exceptions.UbfBEINVALException;
import org.endurox.exceptions.UbfBNOTPRESException;

/**
 * TODO: Might want to add repeated tests so that we can test GC...
 */
public class BaddfastTest {
    
    /**
     * Test field adding to UBF buffer, short type
     */
    @Test
    public void testBaddfastShort() {
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        BFldLocInfo loc = new BFldLocInfo();

        TypedUBF ub = (TypedUBF)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);

        ub.Baddfast(test.T_SHORT_FLD, (short)100, loc);
        ub.Baddfast(test.T_SHORT_FLD, (short)200, loc);
        ub.Baddfast(test.T_SHORT_FLD, (short)300, loc);
        ub.Baddfast(test.T_SHORT_FLD, (short)400, loc);

        /* print some buffer */
        ub.Bprint();

        /* get the field value and verify it */
        assertEquals(100, ub.BgetShort(test.T_SHORT_FLD, 0));
        assertEquals(200, ub.BgetShort(test.T_SHORT_FLD, 1));
        assertEquals(300, ub.BgetShort(test.T_SHORT_FLD, 2));
        assertEquals(400, ub.BgetShort(test.T_SHORT_FLD, 3));
    }
    
    /* test invalid field case */
    @Test(expected = UbfBEINVALException.class)
    public void testBaddfastInvalType() {
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        BFldLocInfo loc = new BFldLocInfo();

        TypedUBF ub = (TypedUBF)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);

        ub.Baddfast(test.T_LONG_FLD, (short)100, loc);
    }
      
    /**
     * Test Long value Get/Set
     */
    @Test
    public void testBaddfastLong() {
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUBF ub = (TypedUBF)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);
        BFldLocInfo loc = new BFldLocInfo();


        ub.Baddfast(test.T_LONG_FLD, (long)888800, loc);
        ub.Baddfast(test.T_LONG_FLD, (long)888811, loc);
        
        /* print some buffer */
        ub.Bprint();

        /* get the field value and verify it */
        assertEquals(888800, ub.BgetLong(test.T_LONG_FLD, 0));
        assertEquals(888811, ub.BgetLong(test.T_LONG_FLD, 1));
    }

    /**
     * Test Long value Get/Set
     */
    @Test
    public void testBaddfastByte() {
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUBF ub = (TypedUBF)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);
        BFldLocInfo loc = new BFldLocInfo();

        ub.Baddfast(test.T_CHAR_FLD, (byte)0, loc);
        ub.Baddfast(test.T_CHAR_FLD, (byte)1, loc);

        /* print some buffer */
        ub.Bprint();

        /* get the field value and verify it */
        assertEquals(0, ub.BgetByte(test.T_CHAR_FLD, 0));
        assertEquals(1, ub.BgetByte(test.T_CHAR_FLD, 1));
    }

    /**
     * Test Long value Get/Set
     */
    @Test
    public void testBaddfastFloat() {
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUBF ub = (TypedUBF)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);

        BFldLocInfo loc = new BFldLocInfo();

        ub.Baddfast(test.T_FLOAT_FLD, (float)0.005, loc);
        ub.Baddfast(test.T_FLOAT_FLD, (float)1.2, loc);

        /* print some buffer */
        ub.Bprint();

        /* get the field value and verify it */
        assertEquals(0.005f, ub.BgetFloat(test.T_FLOAT_FLD, 0), 0.001);
        assertEquals(1.2f, ub.BgetFloat(test.T_FLOAT_FLD, 1), 0.001);
    }

    /**
     * Test Double value Get/Set
     */
    @Test
    public void testBaddfastDouble() {
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUBF ub = (TypedUBF)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);

        BFldLocInfo loc = new BFldLocInfo();

        ub.Baddfast(test.T_DOUBLE_FLD, (double)0.005, loc);
        ub.Baddfast(test.T_DOUBLE_FLD, (double)1.2, loc);

        /* print some buffer */
        ub.Bprint();

        /* get the field value and verify it */
        assertEquals(0.005f, ub.BgetDouble(test.T_DOUBLE_FLD, 0), 0.001);
        assertEquals(1.2f, ub.BgetDouble(test.T_DOUBLE_FLD, 1), 0.001);
    }

    /**
     * Test String value Get/Set
     */
    @Test
    public void testBaddfastString() {
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUBF ub = (TypedUBF)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);

        BFldLocInfo loc = new BFldLocInfo();

        ub.Baddfast(test.T_STRING_FLD, "HELLO", loc);
        ub.Baddfast(test.T_STRING_FLD, "WORLD", loc);

        /* print some buffer */
        ub.Bprint();

        /* get the field value and verify it */
        assertEquals("HELLO", ub.BgetString(test.T_STRING_FLD, 0));
        assertEquals("WORLD", ub.BgetString(test.T_STRING_FLD, 1));
    }

    /**
     * Test Byte array value Get/Set
     */
    @Test
    public void testBaddfastByteArr() {
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUBF ub = (TypedUBF)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);

        BFldLocInfo loc = new BFldLocInfo();

        ub.Baddfast(test.T_CARRAY_FLD, "HELLO".getBytes(), loc);
        ub.Baddfast(test.T_CARRAY_FLD, "WORLD".getBytes(), loc);

        ub.Bprint();

        assertArrayEquals("HELLO".getBytes(), ub.BgetByteArr(test.T_CARRAY_FLD, 0));
        assertArrayEquals("WORLD".getBytes(), ub.BgetByteArr(test.T_CARRAY_FLD, 1));
    }
    
}
