import static org.junit.Assert.*;
import org.junit.Test;
import org.endurox.*;

public class BchgTest {
    
    /**
     * Test change short
     */
    @Test
    public void testBchgShort() {
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);

        short s = 100;
        ub.Bchg(test.T_SHORT_FLD, 0, (short)s);
        ub.Bchg(test.T_SHORT_FLD, 1, (short)201);

        /* print some buffer */
        ub.Bprint();

        /* get the field value and verify it */
        assertEquals(s, ub.BgetShort(test.T_SHORT_FLD, 0));
        assertEquals(201, ub.BgetShort(test.T_SHORT_FLD, 1));
      }
  
    /**
     * Test Long value Get/Set
     */
    @Test
    public void testBchgLong() {
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);

        ub.Bchg(test.T_LONG_FLD, 0, (long)888800);
        ub.Bchg(test.T_LONG_FLD, 1, (long)888811);

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
    public void testBchgByte() {
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);

        ub.Bchg(test.T_CHAR_FLD, 0, (byte)0);
        ub.Bchg(test.T_CHAR_FLD, 1, (byte)1);

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
    public void testBchgFloat() {
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);

        ub.Bchg(test.T_FLOAT_FLD, 0, (float)0.5);
        ub.Bchg(test.T_FLOAT_FLD, 1, (float)1.2);

        /* print some buffer */
        ub.Bprint();

        /* get the field value and verify it */
        ctx.tplogError("Got 0 = %f", ub.BgetFloat(test.T_FLOAT_FLD, 0));
        ctx.tplogError("Got 1 = %f", ub.BgetFloat(test.T_FLOAT_FLD, 1));
        assertEquals(0.5f, ub.BgetFloat(test.T_FLOAT_FLD, 0), 0.01);
        assertEquals(1.2f, ub.BgetFloat(test.T_FLOAT_FLD, 1), 0.01);
    }

    /**
     * Test Double value Get/Set
     */
    @Test
    public void testBchgDouble() {
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);

        ub.Bchg(test.T_DOUBLE_FLD, 0, (double)0.005);
        ub.Bchg(test.T_DOUBLE_FLD, 1, (double)1.2);

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
    public void testBchgString() {
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);

        ub.Bchg(test.T_STRING_FLD, 0, "HELLO");
        ub.Bchg(test.T_STRING_FLD, 1, "WORLD");

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
    public void testBchgByteArr() {
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);

        ub.Bchg(test.T_CARRAY_FLD, 0, "HELLO".getBytes());
        ub.Bchg(test.T_CARRAY_FLD, 1, "WORLD".getBytes());

        ub.Bprint();

        assertArrayEquals("HELLO".getBytes(), ub.BgetByteArr(test.T_CARRAY_FLD, 0));
        assertArrayEquals("WORLD".getBytes(), ub.BgetByteArr(test.T_CARRAY_FLD, 1));
    }
  
}
