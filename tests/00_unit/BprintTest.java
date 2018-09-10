import java.io.BufferedReader;
import java.io.IOException;
import java.io.Reader;
import java.nio.CharBuffer;
import static org.junit.Assert.*;
import org.junit.Test;
import org.endurox.*;
import java.util.Locale;

/**
 * Buffer printing, read & write tests.
 */
public class BprintTest {
    
    
    private class TestExtreadReader extends Reader {

        public TestExtreadReader() {
        }
        
        @Override
        public void close() {
            
        }

        @Override
        public int read(char[] chars, int i, int i1) throws IOException {
            return 0;
        }
    }
    
    private class TestExtread extends BufferedReader {

        public TestExtread(Reader reader, int i) {
            super(reader, i);
        }

        public TestExtread(Reader reader) {
            super(reader);
        }
        
        int idx = -1;
        
        @Override
        public String readLine() throws IOException {
            
            /* return some lines */
            idx++;
            switch (idx)
            {
                case 0:
                    return "T_SHORT_FLD\t88";
                case 1:
                    return "T_SHORT_FLD\t-111\n";
                case 2:
                    return "T_LONG_FLD\t-2\n";
                case 3:
                    return "T_CHAR_FLD\tc\n";
                case 4:
                    return "T_FLOAT_2_FLD\t1227.10000";
                case 5:
                    return "T_DOUBLE_2_FLD\t1233";
                case 6:
                    return "T_STRING_2_FLD\tHELLO";
                case 7:
                    return "T_CARRAY_2_FLD\t\\00\\01\\02";
                default:
                    return null;
            }
        } 
        
    }
    
    /**
     * Test buffer reading from external source from the text buffer
     */
    @Test
    public void testExtread() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
	Locale.setDefault(new Locale("en", "US"));
        
        for (int i=0; i<100; i++)
        {
            TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
            assertNotEquals(ub, null);

            TestExtread r = new TestExtread(new TestExtreadReader());

            /* read buffer from external source */
            ub.Bextread(r);

            ub.Bprint();

            /* verify buffer */
            assertEquals(88, ub.BgetShort(test.T_SHORT_FLD, 0));
            assertEquals(-111, ub.BgetShort(test.T_SHORT_FLD, 1));

            assertEquals(-2, ub.BgetLong(test.T_LONG_FLD, 0));
            assertEquals(0x63, ub.BgetByte(test.T_CHAR_FLD, 0));
            assertEquals(1227.1f, ub.BgetFloat(test.T_FLOAT_2_FLD, 0), 0.01f);
            /* not using Dsep here as we have under java differnt comma seperator
             * thus avoid it at the moment
             */
            assertEquals(1233.0d, ub.BgetDouble(test.T_DOUBLE_2_FLD, 0), 0.01d);
            assertEquals("HELLO", ub.BgetString(test.T_STRING_2_FLD, 0));
            assertArrayEquals(new byte [] {0x00, 0x01, 0x02}, 
                    ub.BgetByteArr(test.T_CARRAY_2_FLD, 0));
        }
    }
    
    /**
     * Test buffer write/read functions
     */
    @Test
    public void testWriteRead() {
        
        AtmiCtx ctx = new AtmiCtx();
        assertNotEquals(ctx.getCtx(), 0x0);
	Locale.setDefault(new Locale("en", "US"));
        
        for (int i=0; i<100; i++)
        {
            TypedUbf ub = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
            assertNotEquals(ub, null);
            
            /* Load UBF data. */
            ButilTest.loadTestData1(ub);
            
            /* write buffer */
            byte[] data = ub.Bwrite();
            
            /* read buffer */
            TypedUbf ub2 = (TypedUbf)ctx.tpalloc("UBF", "", 1024);
            assertNotEquals(ub, null);
            
            ub2.Bread(data);
            ButilTest.validateTestData(ub2);
        }
    }

}
  
