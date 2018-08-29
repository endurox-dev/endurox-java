import java.io.IOException;
import java.io.OutputStream;
import java.util.concurrent.TimeUnit;
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
        assertNotEquals(0x0, ctx.getCtx());
        
        //Comple an expression
        BExprTree tree = ctx.Bboolco(EXPR);
        assertNotEquals(null, tree);
        
        //Print the boolean expression
        ctx.Bboolpr(tree, w);
        
        assertEquals(EXPR, w.output);
        
    }
    
    /**
     * Test Simple version of boolean expression evaluation
     */
    @Test
    public void testBboolev() {
        
        for (int i=0; i<10000; i++)
        {
            AtmiCtx ctx = new AtmiCtx();
            assertNotEquals(ctx.getCtx(), 0x0);

            //Compile an expression
            BExprTree tree = ctx.Bboolco("T_STRING_FLD=='HELLO WORLD' && T_LONG_FLD==551");
            assertNotEquals(tree, null);

            TypedUBF ub = (TypedUBF)ctx.tpalloc("UBF", "", 1024);
            assertNotEquals(ub, null);

            ub.Bchg(test.T_STRING_FLD, 0, "HELLO WORLD");
            ub.Bchg(test.T_LONG_FLD, 0, 551);


            ctx.tplogError("Buffer print ----------------");
            ub.Bprint();
            ctx.tplogError("END ------------------------");

            assertEquals(true, ub.Bboolev(tree));


            ub.Bchg(test.T_STRING_FLD, 0, "HELLO WORLD1");

            assertEquals(false, ub.Bboolev(tree));
            
            ctx.cleanup();
            tree.cleanup();
            ub.cleanup();
        }
        
    }
    
    /**
     * Execute float value evaluation
     */
    @Test
    public void testBfloatev() {
        
        for (int i=0; i<10000; i++)
        {
            AtmiCtx ctx = new AtmiCtx();
            assertNotEquals(ctx.getCtx(), 0x0);

            //Compile an expression
            BExprTree tree = ctx.Bboolco("T_LONG_FLD + 1.3");
            assertNotEquals(null, tree);

            TypedUBF ub = (TypedUBF)ctx.tpalloc("UBF", "", 1024);
            assertNotEquals(null, ub);

            ub.Bchg(test.T_LONG_FLD, 0, 551);


            ctx.tplogError("Buffer print ----------------");
            ub.Bprint();
            ctx.tplogError("END ------------------------");

            
            ctx.tplogError("Got result: %f", ub.Bfloatev(tree));
            
            //assertEquals(552.3, ub.Bfloatev(tree), 0.0001);


            ub.Bchg(test.T_LONG_FLD, 0, 552);

            //assertEquals(552.3, ub.Bfloatev(tree), 0.0001);
            
            ctx.cleanup();
            tree.cleanup();
            ub.cleanup();
        }
        
    }
    
    /**
     * Test Simple version of boolean expression evaluation
     */
    @Test
    public void testBqboolev() {
        
        for (int i=0; i<10000; i++)
        {
            AtmiCtx ctx = new AtmiCtx();
            assertNotEquals(ctx.getCtx(), 0x0);

            TypedUBF ub = (TypedUBF)ctx.tpalloc("UBF", "", 1024);
            assertNotEquals(ub, null);

            ub.Bchg(test.T_STRING_FLD, 0, "HELLO WORLD");
            ub.Bchg(test.T_LONG_FLD, 0, 551);


            ctx.tplogError("Buffer print ----------------");
            ub.Bprint();
            ctx.tplogError("END ------------------------");

            assertEquals(ub.Bqboolev("T_STRING_FLD=='HELLO WORLD' && T_LONG_FLD==551"), true);


            ub.Bchg(test.T_STRING_FLD, 0, "HELLO WORLD1");

            assertEquals(ub.Bqboolev("T_STRING_FLD=='HELLO WORLD' && T_LONG_FLD==551"), false);
            
            ctx.cleanup();
            ub.cleanup();
        }
    }
    /**
     * Callback class for boolean expression evaluation...
     */
    private class GiveMeFive implements Bboolcbf {
        
        /**
         * This function is called as a result of boolean expression evaluation
         * @param ctx ATMI Context into which evaluation is done
         * @param ub UBF buffer onto which expression is evaluated
         * @param funcname function name called
         * @return result long value
         */
        public long bboolCallBack(AtmiCtx ctx, TypedUBF ub, String funcname) {
            
            if (funcname.equals("give_me_five")) {
                return ub.BgetLong(test.T_LONG_FLD, 0);
            } else if (funcname.equals("give_me_six")) {
                return ub.BgetLong(test.T_LONG_2_FLD, 0);
            }
            else
            {
                return -1;
            }
        }
    }
    /**
     * Test Simple version of boolean expression evaluation
     */
    @Test
    public void testBboolsetcbf() {
        
        AtmiCtx ctx = new AtmiCtx();
        GiveMeFive cb = new GiveMeFive();
        
        assertNotEquals(ctx.getCtx(), 0x0);
            
        ctx.Bboolsetcbf ("give_me_five", cb);
        ctx.Bboolsetcbf ("give_me_six", cb);
        
        for (int i=0; i<1000; i++)
        {
            BExprTree tree = ctx.Bboolco("give_me_five()==5 && give_me_six()==6");
            assertNotEquals(tree, null);

            TypedUBF ub = (TypedUBF)ctx.tpalloc("UBF", "", 1024);
            assertNotEquals(ub, null);

            ub.Bchg(test.T_LONG_FLD, 0, 5);
            ub.Bchg(test.T_LONG_2_FLD, 0, 6);

            ctx.tplogError("Buffer print ----------------");
            ub.Bprint();
            ctx.tplogError("END ------------------------");

            assertEquals(ub.Bboolev(tree), true);

            ub.Bchg(test.T_LONG_2_FLD, 0, 7);

            assertEquals(ub.Bboolev(tree), false);

            ctx.tplogError("Buffer print ----------------");
            ub.Bprint();
            ctx.tplogError("END ------------------------");
            
            tree.cleanup();
            ub.cleanup();
        }
    }    
}
