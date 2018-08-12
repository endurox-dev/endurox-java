import static org.junit.Assert.*;
import org.junit.Test;
import org.endurox.*;
import org.endurox.exceptions.UbfBNOTPRESException;

public class BaddTest {
    
    /**
     * Test field adding to UBF buffer, short type
     */
    @Test
    public void testBaddShort() {
      AtmiCtx ctx = new AtmiCtx();
      assertNotEquals(ctx.getCtx(), 0x0);
      TypedUBF ub = (TypedUBF)ctx.tpalloc("UBF", "", 1024);
      assertNotEquals(ub, null);

      short s = 100;
      ub.Badd(test.T_SHORT_FLD, s);
      ub.Badd(test.T_SHORT_FLD, (short)201);

      /* print some buffer */
      ub.Bprint();

      /* get the field value and verify it */
      assertEquals(s, ub.BgetShort(test.T_SHORT_FLD, 0));
      assertEquals(201, ub.BgetShort(test.T_SHORT_FLD, 1));
    }
  
    @Test(expected = UbfBNOTPRESException.class)
    public void testBgetNotPres() {
        
      AtmiCtx ctx = new AtmiCtx();
      assertNotEquals(ctx.getCtx(), 0x0);
      TypedUBF ub = (TypedUBF)ctx.tpalloc("UBF", "", 1024);
      assertNotEquals(ub, null);
      
      // This will throw exception
      ub.BgetShort(test.T_SHORT_FLD, 0);
      
    }
  
    /**
     * Test Long value Get/Set
     */
    @Test
    public void testBaddLong() {
      AtmiCtx ctx = new AtmiCtx();
      assertNotEquals(ctx.getCtx(), 0x0);
      TypedUBF ub = (TypedUBF)ctx.tpalloc("UBF", "", 1024);
      assertNotEquals(ub, null);

      ub.Badd(test.T_LONG_FLD, (long)888800);
      ub.Badd(test.T_LONG_FLD, (long)888811);

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
    public void testBaddByte() {
      AtmiCtx ctx = new AtmiCtx();
      assertNotEquals(ctx.getCtx(), 0x0);
      TypedUBF ub = (TypedUBF)ctx.tpalloc("UBF", "", 1024);
      assertNotEquals(ub, null);

      ub.Badd(test.T_CHAR_FLD, (byte)0);
      ub.Badd(test.T_CHAR_FLD, (byte)1);

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
    public void testBaddFloat() {
      AtmiCtx ctx = new AtmiCtx();
      assertNotEquals(ctx.getCtx(), 0x0);
      TypedUBF ub = (TypedUBF)ctx.tpalloc("UBF", "", 1024);
      assertNotEquals(ub, null);

      ub.Badd(test.T_FLOAT_FLD, (float)0.5);
      ub.Badd(test.T_FLOAT_FLD, (float)1.2);

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
    public void testBaddDouble() {
      AtmiCtx ctx = new AtmiCtx();
      assertNotEquals(ctx.getCtx(), 0x0);
      TypedUBF ub = (TypedUBF)ctx.tpalloc("UBF", "", 1024);
      assertNotEquals(ub, null);

      ub.Badd(test.T_DOUBLE_FLD, (double)0.005);
      ub.Badd(test.T_DOUBLE_FLD, (double)1.2);

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
    public void testBaddString() {
      AtmiCtx ctx = new AtmiCtx();
      assertNotEquals(ctx.getCtx(), 0x0);
      TypedUBF ub = (TypedUBF)ctx.tpalloc("UBF", "", 1024);
      assertNotEquals(ub, null);

      ub.Badd(test.T_STRING_FLD, "HELLO");
      ub.Badd(test.T_STRING_FLD, "WORLD");

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
    public void testBaddByteArr() {
      AtmiCtx ctx = new AtmiCtx();
      assertNotEquals(ctx.getCtx(), 0x0);
      TypedUBF ub = (TypedUBF)ctx.tpalloc("UBF", "", 1024);
      assertNotEquals(ub, null);

      ub.Badd(test.T_CARRAY_FLD, "HELLO".getBytes());
      ub.Badd(test.T_CARRAY_FLD, "WORLD".getBytes());

      ub.Bprint();

      assertArrayEquals("HELLO".getBytes(), ub.BgetByteArr(test.T_CARRAY_FLD, 0));
      assertArrayEquals("WORLD".getBytes(), ub.BgetByteArr(test.T_CARRAY_FLD, 1));
    }
  
}
