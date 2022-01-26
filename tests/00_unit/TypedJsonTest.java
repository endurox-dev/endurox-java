import static org.junit.Assert.*;
import org.junit.Test;
import org.endurox.*;
import org.endurox.exceptions.*;

/**
 * Typed JSON buffer tests
 */
public class TypedJsonTest {

    public static final String BIGTEST = "THIS IS SOME VERY BIG STRNIG 1" +
                   "THIS IS SOME VERY BIG STRNIG 2" +
                   "THIS IS SOME VERY BIG STRNIG 3";
    /**
     * Test string allocation
     */
    @Test
    public void testJsonBasic() {

        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedJson jsn;

        for (int i=0; i<2000; i++) {
            
            String cmp = "{\"test\":\"";
            
            /* seems on some jdk version too long concat is slow */
            for (int j=0; j<10; j++)
            {
                cmp = cmp.concat(BIGTEST);
            }
            
            cmp = cmp.concat("\"}");
            
            /* create new string */
            jsn = ctx.newJson("[]");
            
            /* get the string... */
            assertEquals("[]", jsn.getJSON());
            
            /* Allocate some bigger string...! */
            jsn.setJSON(cmp);
            
            /* check the value... */
            assertEquals(cmp, jsn.getJSON());
            
            /* str.cleanup(); */
        }
        
        /* check that we got gc...! */
        assertFalse(ctx.getBuffers().length == 1000);
        
        ctx.cleanup();
    }
}
