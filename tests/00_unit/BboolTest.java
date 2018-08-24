import java.io.IOException;
import java.io.OutputStream;
import org.junit.Test;
import static org.junit.Assert.*;
import org.endurox.*;

/**
 * Boolean expressions testing
 */
public class BboolTest {
    
    
    class BboolprWritter extends OutputStream {

        public String output = "";
        @Override
        public void close() throws IOException {
            super.close(); //To change body of generated methods, choose Tools | Templates.
        }

        @Override
        public void flush() throws IOException {
            super.flush(); //To change body of generated methods, choose Tools | Templates.
        }

        @Override
        public void write(byte[] bytes, int i, int i1) throws IOException {
            super.write(bytes, i, i1); //To change body of generated methods, choose Tools | Templates.
        }

        @Override
        public void write(byte[] bytes) throws IOException {
            String str = new String(bytes).split("\0")[0].replaceAll("\n", "").replaceAll(" ", "");
            output = output.concat(str);
        }
        
        @Override
        public void write(int i) throws IOException {
        }
        
    }
    public static final String EXPR = "((1==1)||((5!=6)&&1))";
            
    /**
     * Test boolean expression printing
     */
    @Test
    public void testBboolpr() {
        
        AtmiCtx ctx = new AtmiCtx();
        BboolprWritter w = new BboolprWritter();
        assertNotEquals(ctx.getCtx(), 0x0);
        
        //Comple an expression
        BExprTree tree = ctx.Bboolco(EXPR);
        assertNotEquals(tree, null);
        
        //Print the boolean expression
        ctx.Bboolpr(tree, w);
        
        assertEquals(EXPR, w.output);
        
    }
}

