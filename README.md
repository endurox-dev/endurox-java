Before build set JAVA_HOME so that cmake can find the jni.h


### Flags:


- To enable Oracle JDBC Testing with '-DENABLE_XAORACLE02=ON'
Note! ~/ndrx_home must contain the valid EX_ORA_ settings.
And tests/02_xaoracle/jdbcdrivers folder must be loaded with Oracle
JDBC driver jars. Particulary *ojdbc8.jar*.
