import static org.junit.Assert.*;
import org.junit.Test;
import org.endurox.*;
import org.endurox.exceptions.*;

/**
 * Typed buffer tests (commons)
 */
public class TypedBufferTest {

    /**
     * Test the tptypes() command of TypedBuffer
     */
    @Test
    public void testTpTypes() {

        AtmiCtx ctx = new AtmiCtx();
        for (int i=0; i<100; i++) {
            
            TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 2048);
            TpTypesResult res = ub.tptypes();
            
            assertEquals("UBF", res.getType());
            assertEquals("", res.getSubType());
            assertEquals(2048, res.getSize());
            
        }

    }
}
