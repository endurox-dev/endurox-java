import static org.junit.Assert.*;
import org.junit.Test;
import org.endurox.*;
import org.endurox.exceptions.*;

/**
 * Typed JSON buffer tests
 */
public class TypedJsonTest {

    /* over 1024 bytes */
    public static final String BIGTEST = "THIS IS SOME VERY BIG STRING 1" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 2" +
                   "THIS IS SOME VERY BIG STRING 3";
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
            
            cmp.concat(BIGTEST);
            
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
        
        ctx.cleanup();
    }
}
