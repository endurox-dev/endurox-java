Before build set JAVA_HOME so that cmake can find the jni.h


## Configure make with: 

$ cmake -DCMAKE_INSTALL_PREFIX:PATH=`pwd`/dist .

### Flags:

- To enable Oracle JDBC Testing, configure with '-DENABLE_TEST_XAORACLE=ON'

- To enable PostgreSQL Testing, configure with '-DENABLE_TEST_XAPOSGRES=ON'

