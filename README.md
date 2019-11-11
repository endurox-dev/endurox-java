# Enduro/X Java Application server

This project provides Plain Old Java Objects (POJO) application server. At the
heart of the project Enduro/X Application Server is used, which uses kernel queues
for inter-process communications (IPC), thus high performance throughput can be
reached. Application has also C and Go API, thus the whole application can
be built in these three languages. As Java supports XA transactions, Enduro/X
supports them too. Thus is possible to perform two-phase-commit, distributed
transactions over the C/Go and Java processes.

More about Enduro/X middleware can be seen here: https://github.com/endurox-dev/endurox

To download already precompiled versions of Enduro/X Core and Java, visit:
https://www.endurox.org/projects/endurox/files

Javadoc and other documentation for the package are available here: 
https://www.endurox.org/dokuwiki/doku.php?id=endurox-java:v1.0.x:api:javadoc

## Overview

This is binding project for Enduro/X microservices XATMI API based framework. Bindings
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

- Each JVM instanced can be tweaked individually.

- Different classes/XATMI services can have different set of dependencies and
and different dependency versions.

- If using compiled Java binaries, the whole application can be built in such
way that it is forwards and backwards compatible with newer and older binaries.

From API perspective, Enduro/X Java module provides full set of XATMI APIs:

Which includes:

- XATMI IPC syncrhonus, asyncrhouns(tpadvertise(), tpcall(), tpforward(), etc).

- XATMI Transactions

- UBF/FML buffers API (Badd, Bchg, Bboolco(), etc.)

- Generic JSON/String/Carray buffers


## IPC Concept


## Protocol buffers



## License

The license for project is LGPL for Java code and AGPL with Java exception for
C code. The Enduro/X Core license permits Java and Go languages to use Enduro/X
Core as an LGPL project. Thus code written in Java can be included by free of
charge in the commercial products. Still the Mavimax Ltd would be happy to provide
commercial support.


## Configure make with: 

$ cmake -DCMAKE_INSTALL_PREFIX:PATH=`pwd`/dist .

### Flags:

- To enable Oracle JDBC Testing, configure with '-DENABLE_TEST_XAORACLE=ON'

- To enable PostgreSQL Testing, configure with '-DENABLE_TEST_XAPOSGRES=ON'

- To disable doc building, configure with '-DDEFINE_DISABLEDOC=ON'
