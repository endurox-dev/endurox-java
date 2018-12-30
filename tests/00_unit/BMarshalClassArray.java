import org.junit.Test;
import static org.junit.Assert.*;
import org.endurox.*;
import java.lang.Short;
import java.lang.Long;
import java.lang.Byte;
import java.lang.Float;
import java.lang.Double;
/**
 * Marshal testing class
 * will list all fields supported fields here
 */
public class BMarshalClassArray {
             
    /* standard types: */
    @UbfField(bfldid=test.T_SHORT_FLD/*, ubfmin=1, ojbmin = 0*/)
    short[] tshort;
    @UbfField(bfldid=test.T_SHORT_2_FLD)
    Short[] tshort2;
    
    @UbfField(bfldid=test.T_LONG_FLD)
    long[] tlong;
    @UbfField(bfldid=test.T_LONG_2_FLD)
    Long[] tlong2;
    
    @UbfField(bfldid=test.T_CHAR_FLD)
    byte[] tchar;
    @UbfField(bfldid=test.T_CHAR_2_FLD)
    Byte[] tchar2;
    
    @UbfField(bfldid=test.T_FLOAT_FLD)
    float[] tfloat;
    @UbfField(bfldid=test.T_FLOAT_2_FLD)
    Float[] tfloat2;
    
    @UbfField(bfldid=test.T_DOUBLE_FLD)
    double[] tdouble;
    @UbfField(bfldid=test.T_DOUBLE_2_FLD)
    Double[] tdouble2;
    
    @UbfField(bfldid=test.T_STRING_FLD)
    String[] tstring;
    
    @UbfField(bfldid=test.T_CARRAY_FLD)
    byte [][] tcarray;
    
    @UbfField(bfldid=test.T_CARRAY_2_FLD)
    Byte [][] tcarray2;

    public Byte[] getTchar2() {
        return tchar2;
    }

    public void setTchar2(Byte[] tchar2) {
        this.tchar2 = tchar2;
    }

    public Byte[][] getTcarray2() {
        return tcarray2;
    }

    public void setTcarray2(Byte[][] tcarray2) {
        this.tcarray2 = tcarray2;
    }

    public short[] getTshort() {
        return tshort;
    }

    public void setTshort(short[] tshort) {
        this.tshort = tshort;
    }

    public Short[] getTshort2() {
        return tshort2;
    }

    public void setTshort2(Short[] tshort2) {
        this.tshort2 = tshort2;
    }

    public long[] getTlong() {
        return tlong;
    }

    public void setTlong(long[] tlong) {
        this.tlong = tlong;
    }

    public Long[] getTlong2() {
        return tlong2;
    }

    public void setTlong2(Long[] tlong2) {
        this.tlong2 = tlong2;
    }

    public byte[] getTchar() {
        return tchar;
    }

    public void setTchar(byte[] tchar) {
        this.tchar = tchar;
    }

    public float[] getTfloat() {
        return tfloat;
    }

    public void setTfloat(float[] tfloat) {
        this.tfloat = tfloat;
    }

    public Float[] getTfloat2() {
        return tfloat2;
    }

    public void setTfloat2(Float[] tfloat2) {
        this.tfloat2 = tfloat2;
    }

    public double[] getTdouble() {
        return tdouble;
    }

    public void setTdouble(double[] tdouble) {
        this.tdouble = tdouble;
    }

    public Double[] getTdouble2() {
        return tdouble2;
    }

    public void setTdouble2(Double[] tdouble2) {
        this.tdouble2 = tdouble2;
    }

    public String[] getTstring() {
        return tstring;
    }

    public void setTstring(String[] tstring) {
        this.tstring = tstring;
    }

    public byte[][] getTcarray() {
        return tcarray;
    }

    public void setTcarray(byte[][] tcarray) {
        this.tcarray = tcarray;
    }
    
    /**
     * Load test data
     * @return prepared BMarshalClassArray object
     */
    public static BMarshalClassArray getTestData() {
        
        BMarshalClassArray ret = new BMarshalClassArray();
        
        /* short tests */
        ret.tshort = new short[3];
        ret.tshort[0] = 155;
        ret.tshort[1] = 667;
        ret.tshort[2] = -225;
        
        ret.tshort2 = new Short[4];
        ret.tshort2[0] = -155;
        ret.tshort2[1] = 0;
        ret.tshort2[2] = 225;
        ret.tshort2[3] = 2;
        
        /* long tests */
        
        ret.tlong = new long[2];
        ret.tlong[0] = 100000000L;
        ret.tlong[1] = -1000000000L;
        
        ret.tlong2 = new Long[1];
        ret.tlong2[0] = -99999999L;
        
        return ret;
    }
}

