import static org.junit.Assert.*;
import org.junit.Test;
import org.endurox.*;
import org.endurox.exceptions.*;

/**
 * Basic ATMI tests... (mostly non IPC related)
 * @author mvitolin
 */
public class AtmiCtxTest {

    /**
     * Test object allocation, buffer alloc
     */
    @Test
    public void newCtx() {

        for (int i=0; i<100; i++) {
            AtmiCtx ctx = new AtmiCtx();
            assertNotEquals(ctx.getCtx(), 0x0);
            TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
            assertNotEquals(ub, null);

            /* test sub-type NULL */
            ub = (TypedUbf)ctx.tpalloc("UBF", null, 1024);
            assertNotEquals(ub, null);
            ctx.tplogInfo("Hello Log");

            ub.cleanup();
            ctx.cleanup();
        }

    }

    /**
    * Test invalid buffer type exception
    */
    @Test(expected = org.endurox.exceptions.AtmiTPEINVALException.class)
    public void testInvalidBuffer() {
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedUbf ub = (TypedUbf)ctx.tpalloc(null, "", 1024);
    }

    /**
    * Test Btype method
    */
    @Test
    public void testBtype() {
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(0x0, ctx.getCtx());

        assertEquals("short", ctx.Btype(test.T_SHORT_FLD));
        assertEquals("long", ctx.Btype(test.T_LONG_FLD));
        assertEquals("char", ctx.Btype(test.T_CHAR_FLD));
        assertEquals("float", ctx.Btype(test.T_FLOAT_FLD));
        assertEquals("double", ctx.Btype(test.T_DOUBLE_FLD));
        assertEquals("string", ctx.Btype(test.T_STRING_FLD));
        assertEquals("carray", ctx.Btype(test.T_CARRAY_FLD));
    }

    /**
    * Btype error
    */
    @Test(expected = org.endurox.exceptions.UbfBTYPERRException.class)
    public void testBtypeError() {

        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(0x0, ctx.getCtx());
        ctx.Btype(-1);
    }

    /**
    * Test Bfname
    */
    @Test
    public void testBfname() {
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(0x0, ctx.getCtx());

        assertEquals("T_SHORT_FLD", ctx.Bfname(test.T_SHORT_FLD));
    }
    
    /**
    * Test Bfldid
    */
    @Test
    public void testBfldid() {
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(0x0, ctx.getCtx());

        assertEquals(test.T_SHORT_FLD, ctx.Bfldid("T_SHORT_FLD"));
    }
    
    /**
     * Test invalid field id / NULL
     */
    @Test(expected = org.endurox.exceptions.UbfBEINVALException.class)
    public void testBfldidNull() {
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(0x0, ctx.getCtx());

        assertEquals(test.T_SHORT_FLD, ctx.Bfldid(null));
    }

    /**
    * Test Bfname, error
    */
    @Test(expected = org.endurox.exceptions.UbfBEINVALException.class)
    public void testBfnameError() {

        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(0x0, ctx.getCtx());
        ctx.Bfname(-1);
    }

}
