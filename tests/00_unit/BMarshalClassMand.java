import org.endurox.*;
import java.lang.Short;
import java.lang.Long;
import java.lang.Byte;
import java.lang.Float;
import java.lang.Double;

/**
 * Test basically mandatory field
 */
public class BMarshalClassMand {
             
    /* standard types: */
    @UbfField(bfldid=test.T_SHORT_FLD, ubfmin=1, ojbmin=1)
    short tshort;
}
