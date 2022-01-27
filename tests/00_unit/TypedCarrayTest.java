import static org.junit.Assert.*;
import org.junit.Test;
import org.endurox.*;
import org.endurox.exceptions.*;

/**
 * Typed C Array (byte array) test
 */
public class TypedCarrayTest {

    /**
     * Test carray buffer processing
     */
    @Test
    public void testCarrayBasic() {

        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedCarray ca;

        for (int i=0; i<2000; i++) {
            
            byte [] ba1 = new byte[] {1, 2, 3};
            
            ca = ctx.newCarray(ba1);
            
            
            assertArrayEquals(ba1, ca.getBytes());

            byte [] ba2 = new byte[i];
            for (int j=0; j<i; j++)
            {
                ba2[j] = (byte)(j % 100);
            }
            
            ca.setBytes(ba2);
            assertArrayEquals(ba2, ca.getBytes());
            
        }
        
        /* check that we got gc...! 
        assertFalse(ctx.getBuffers().length == 1000);
        */
        
        ctx.cleanup();
    }
}
