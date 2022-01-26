import static org.junit.Assert.*;
import org.junit.Test;
import org.endurox.*;
import org.endurox.exceptions.*;

/**
 * Typed string tests
 */
public class TypedStringTest {

    
    public static final String BIGTEST = "THIS IS SOME VERY BIG STRNIG 1" +
                   "THIS IS SOME VERY BIG STRNIG 2" +
                   "THIS IS SOME VERY BIG STRNIG 3";
    
    /**
     * Test string alloocation
     */
    @Test
    public void testStringBasic() {

        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
        TypedString str;

        for (int i=0; i<1000; i++) {
            
            String cmp = BIGTEST;
            
            for (int j=0; j<10; j++)
            {
                cmp = cmp.concat(BIGTEST);
            }
            
            /* create new string */
            str = ctx.newString("HELLO WORLD");
            
            /* get the string... */
            assertEquals("HELLO WORLD", str.getString());
            
            /* Allocate some bigger string...! */
            str.setString(cmp);
            
            /* check the value... */
            assertEquals(cmp, str.getString());
            
            /* str.cleanup(); */
        }
        
        /* check that we got gc...! */
        assertFalse(ctx.getBuffers().length == 1000);
        
        ctx.cleanup();
    }
}

