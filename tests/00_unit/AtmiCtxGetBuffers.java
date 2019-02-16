import static org.junit.Assert.*;
import org.junit.Test;
import org.endurox.*;
import org.endurox.exceptions.*;

/**
 * Get buffers test
 */
public class AtmiCtxGetBuffers {

    /**
     * Return test the list of buffers
     */
    @Test
    public void testGetBuffers() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        
        assertEquals(0, ctx.getBuffers().length);
        
        TypedString s = ctx.newString("HELLO STRING");
        TypedUbf u = (TypedUbf)ctx.tpalloc("UBF", null, 1000);
        
        assertEquals(2, ctx.getBuffers().length);

    }
}
