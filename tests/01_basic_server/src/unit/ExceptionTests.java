import org.junit.Test;
import static org.junit.Assert.*;
import org.endurox.*;
import org.endurox.exceptions.AtmiTPESVCERRException;

/**
 * Test service exception
 */
public class ExceptionTests {
             
    /**
     * Except tests, shall respond with 
     */
    @Test(expected = AtmiTPESVCERRException.class)
    public void tpcallTest() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);

        TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
        assertNotEquals(ub, null);
        
        ctx.tpcall("EXCEPT", ub, 0);
        
        ctx.cleanup();
    }
    
}
