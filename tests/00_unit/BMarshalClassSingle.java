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
public class BMarshalClassSingle {
             
    /* standard types: */
    @UbfField(bfldid=test.T_SHORT_FLD)
    short tshort;
    @UbfField(bfldid=test.T_SHORT_FLD)
    Short tshort2;
    
    @UbfField(bfldid=test.T_LONG_FLD)
    long tlong;
    @UbfField(bfldid=test.T_LONG_FLD)
    Long tlong2;
    
    @UbfField(bfldid=test.T_CHAR_FLD)
    byte tchar;
    @UbfField(bfldid=test.T_CHAR_FLD)
    Byte tchar2;
    
    @UbfField(bfldid=test.T_FLOAT_FLD)
    float tfloat;
    @UbfField(bfldid=test.T_FLOAT_FLD)
    Float tfloat2;
    
    @UbfField(bfldid=test.T_DOUBLE_FLD)
    double tdouble;
    @UbfField(bfldid=test.T_DOUBLE_FLD)
    Double tdouble2;
    
    @UbfField(bfldid=test.T_STRING_FLD)
    String tstring;

    public byte[][] getTcarray() {
        return tcarray;
    }

    public void setTcarray(byte[][] tcarray) {
        this.tcarray = tcarray;
    }

    public Byte[][] getTcarray2() {
        return tcarray2;
    }

    public void setTcarray2(Byte[][] tcarray2) {
        this.tcarray2 = tcarray2;
    }
    
    /**
     * NOTE! 
     * CARRAY Is double array by default!
     */
    @UbfField(bfldid=test.T_CARRAY_FLD)
    byte [][] tcarray;
    
    @UbfField(bfldid=test.T_CARRAY_FLD)
    Byte [][] tcarray2;
    
    public short getTshort() {
        return tshort;
    }

    public void setTshort(short tshort) {
        this.tshort = tshort;
    }

    public Short getTshort2() {
        return tshort2;
    }

    public void setTshort2(Short tshort2) {
        this.tshort2 = tshort2;
    }

    public long getTlong() {
        return tlong;
    }

    public void setTlong(long tlong) {
        this.tlong = tlong;
    }

    public Long getTlong2() {
        return tlong2;
    }

    public void setTlong2(Long tlong2) {
        this.tlong2 = tlong2;
    }

    public byte getTchar() {
        return tchar;
    }

    public void setTchar(byte tchar) {
        this.tchar = tchar;
    }

    public Byte getTchar2() {
        return tchar2;
    }

    public void setTchar2(Byte tchar2) {
        this.tchar2 = tchar2;
    }

    public float getTfloat() {
        return tfloat;
    }

    public void setTfloat(float tfloat) {
        this.tfloat = tfloat;
    }

    public Float getTfloat2() {
        return tfloat2;
    }

    public void setTfloat2(Float tfloat2) {
        this.tfloat2 = tfloat2;
    }

    public double getTdouble() {
        return tdouble;
    }

    public void setTdouble(double tdouble) {
        this.tdouble = tdouble;
    }

    public Double getTdouble2() {
        return tdouble2;
    }

    public void setTdouble2(Double tdouble2) {
        this.tdouble2 = tdouble2;
    }

    public String getTstring() {
        return tstring;
    }

    public void setTstring(String tstring) {
        this.tstring = tstring;
    }
    
}
