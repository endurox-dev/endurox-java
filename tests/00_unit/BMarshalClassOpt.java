import org.endurox.*;

/**
 * Test optional field
 */
public class BMarshalClassOpt {
             
    /* standard types: */
    @UbfField(bfldid=test.T_SHORT_FLD, ubfmin=0, ojbmin=0)
    short tshort;

    public short getTshort() {
        return tshort;
    }

    public void setTshort(short tshort) {
        this.tshort = tshort;
    }
}
