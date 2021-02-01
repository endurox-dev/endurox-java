# Enduro/X Java Application server

This project provides Plain Old Java Objects (POJO) application server. At the
heart of the project Enduro/X Application Server is used, which uses kernel queues
for inter-process communications (IPC), thus high performance throughput can be
reached. Application has also C and Go API, thus the whole application can
be built in these three languages. As Java supports XA transactions, Enduro/X
supports them too. Thus is possible to perform two-phase-commit, distributed
transactions over the C/Go and Java processes.

![Alt text](doc/img/endurox-java.png?raw=true "Enduro/X Multi Processing model")

More about Enduro/X middleware can be seen here: https://github.com/endurox-dev/endurox

To download already precompiled versions of Enduro/X Core and Java, visit:
https://www.endurox.org/projects/endurox/files

Javadoc and other documentation for the package are available here: 
https://www.endurox.org/dokuwiki/doku.php?id=endurox-java:v1.0.x:api:javadoc


## Build & test status

| OS   |      Status      | OS       |      Status   |OS       |      Status   |
|----------|:-------------:|----------|:-------------:|----------|:-------------:|
| AIX 7.1 (JDK 1.8)|  [![Build Status](http://www.silodev.com:9090/jenkins/buildStatus/icon?job=endurox-java-aix7_1)](http://www.silodev.com:9090/jenkins/job/endurox-java-aix7_1/) |Centos 6 (JDK 1.7)|[![Build Status](http://www.silodev.com:9090/jenkins/buildStatus/icon?job=endurox-java-centos6)](http://www.silodev.com:9090/jenkins/job/endurox-java-centos6/)|FreeBSD 11|[![Build Status](http://www.silodev.com:9090/jenkins/buildStatus/icon?job=endurox-java-freebsd11)](http://www.silodev.com:9090/jenkins/job/endurox-java-freebsd11/)|
|Oracle Linux 7 (JDK 1.8)|[![Build Status](http://www.silodev.com:9090/jenkins/buildStatus/icon?job=endurox-java-ol7)](http://www.silodev.com:9090/jenkins/job/endurox-java-ol7/)|OSX 11.4 (JDK 1.8)|[![Build Status](http://www.silodev.com:9090/jenkins/buildStatus/icon?job=endurox-java-osx11_4)](http://www.silodev.com:9090/jenkins/job/endurox-java-osx11_4/)|raspbian10_arv7l (JDK 11)|[![Build Status](http://www.silodev.com:9090/jenkins/buildStatus/icon?job=endurox-java-raspbian10_arv7l)](http://www.silodev.com:9090/jenkins/job/endurox-java-raspbian10_arv7l/)|
|RHEL/Oracle Linux 8 (JDK 11)| [![Build Status](http://www.silodev.com:9090/jenkins/buildStatus/icon?job=endurox-java-ol8)](http://www.silodev.com:9090/jenkins/job/endurox-java-ol8/)|SLES 15 (JDK 10)|[![Build Status](http://www.silodev.com:9090/jenkins/buildStatus/icon?job=endurox-java-sles15)](http://www.silodev.com:9090/jenkins/job/endurox-java-sles15/)|Solaris 10 (JDK 1.8)|[![Build Status](http://www.silodev.com:9090/jenkins/buildStatus/icon?job=endurox-java-solaris10_sparc)](http://www.silodev.com:9090/jenkins/job/endurox-java-solaris10-sparc/)|
|Solaris 11 (JDK 1.8)| [![Build Status](http://www.silodev.com:9090/jenkins/buildStatus/icon?job=endurox-java-solaris11_x86)](http://www.silodev.com:9090/jenkins/job/endurox-java-solaris11_x86/)|Ubuntu 14.04 (JDK 1.7)| [![Build Status](http://www.silodev.com:9090/jenkins/buildStatus/icon?job=endurox-java-ubuntu14)](http://www.silodev.com:9090/jenkins/job/endurox-java-ubuntu14/)|Ubuntu 18.04 (JDK 11)| [![Build Status](http://www.silodev.com:9090/jenkins/buildStatus/icon?job=endurox-java-ubuntu18)](http://www.silodev.com:9090/jenkins/job/endurox-java-ubuntu18/)|


## Overview

This is binding project for Enduro/X micro-services XATMI API based framework. Bindings
allow to write POJO style source code. The output from the Java source might be
set of jar files or set of executables (linked with package provided linker). XATMI
API basically provides client-server architecture, where the class can take a
role as an server and advertise some services to the system, or class can be a
XATMI client and consume the services.

The Enduro/X Core package enables to configure whole system application server, 
define which classes/jars/binaries to boot. Enduro/X manages the desired cluster
state, restarts dead instances, etc.

The base technology for Enduro/X Java POJO Application server is, that every
instance of the Java jar or executable are started in separate JVM instance.
The classes can be loaded balanced, started in several copies, and that would
mean that several JVMs are booted. From memory usage aspect this is overhead,
but as the Enduro/X Core by it self makes a very small footprint, and if the
program is written in such way that booted classes only load the required
dependencies, the memory usage can be acceptable.

On the other hand, multiple JVMs gives much more advantages:

- The whole application becomes crash safe. If any class causes corruption,
memory leaks, etc, the Enduor/X Core will boot the particular set of classes
with the corresponding JVMs. The other JVMs will keep serving the application.

- Easy to build multi-threaded systems. Just boot the more process copies. No
worry about thread synchronization, etc. All is done a the middleware level.

- Each JVM instanced can be tweaked individually.

- Different classes/XATMI services can have different set of dependencies and
and different dependency versions.

- If using compiled Java binaries, the whole application can be built in such
way that it is forwards and backwards compatible with newer and older binaries.

From API perspective, Enduro/X Java module provides full set of XATMI APIs and
features:

- Ultra fast XATMI IPC: synchronous, asynchronous(tpadvertise(), tpcall(), tpforward(), etc).

- XATMI XA Transactions

- UBF/FML buffers API (Badd, Bchg, Bboolco(), etc.)

- Generic JSON/String/Carray buffers

- Out of the box clusterization, full abstraction from the programmer perspective.

- Logging framework (tplog(), etc.)

- Configuration facility (ini files driving)

- Strong monitoring possibilities, monitor processes, services, queues, etc with
help of NetXMS tool.

Release binaries may be found here: https://www.mavimax.com/downloads


## License

The license for project is LGPL for Java code and AGPL with Java exception for
C code. The Enduro/X Core license permits Java and Go languages to use Enduro/X
Core as an LGPL project. Thus code written in Java can be included by free of
charge in the commercial products. Still the Mavimax Ltd would be happy to provide
commercial support.


# Releases

- Version 1.0.2 released on 26/11/2019. Initial release
- Version 1.0.4 released on 27/12/2019. Fixes for Bug #490
- Version 1.0.5 released on 10/07/2020. Fixed Bug #569
- Version 1.0.6 / 1.5.2 released on 17/08/2020. Support #583
- Version 1.5.4 released on 01/02/2021. Feature #567

## Configure make with: 

$ cmake -DCMAKE_INSTALL_PREFIX:PATH=`pwd`/dist .

### Flags:

- To enable Oracle JDBC Testing, configure with '-DENABLE_TEST_XAORACLE=ON'

- To enable PostgreSQL Testing, configure with '-DENABLE_TEST_XAPOSGRES=ON'

- To disable doc building, configure with '-DDEFINE_DISABLEDOC=ON'

- To enable strict mutex checking on GNU platform, use '-DMUTEX_DEBUG=ON' (for
GNU platforms only)

