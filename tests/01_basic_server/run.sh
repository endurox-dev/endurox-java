#!/bin/bash

#
# @(#) Run the test case
#

#
# Ask for JMV path.., if needed
#

ADDLIBPATH=""

if [ ! -f  conf/settest1 ]; then
    
    echo "Please enter libjvm.so libpath "
    echo "(e.g. /usr/lib/jvm/java-8-openjdk-amd64/jre/lib/amd64:/usr/lib/jvm/java-8-openjdk-amd64/jre/lib/amd64/server): "
    read lpath

    ADDLIBPATH=":$lpath"

fi

#
# Have some runtime 
#
xadmin provision -d -vaddubf=test.fd -vviewInstall=y -vviewFiles=jview01.V \
	-vshLibs=`pwd`/../../libsrc/c:`pwd`/../../libexjlds$ADDLIBPATH

export ASAN_OPTIONS=handle_segv=0

pushd .
cd conf
. settest1
popd

# Start the enduro/x app server (which will boot the our server executable)

#
# Generic exit function
#
function go_out {
    echo "Test exiting with: $1"
    
    xadmin stop -y
    xadmin down -y 2>/dev/null

    exit $1
}

# debug on
export NDRX_CCTAG="on"

xadmin start -y
rm /tmp/01_CLIENT.log 2>/dev/null

#
# Running client 01
#
echo "Running jclient01b..."
jclient01b > /tmp/01_CLIENT.log 2>&1

RET=$?

if [ "X$RET" != "X0" ]; then
	echo "jclient01b failed"
	go_out 1
fi

#
# Running unit tests...
#

xadmin psc

# Valgrind debug:
#  valgrind --main-stacksize=9916777216 --leak-check=full --show-leak-kinds=all --show-reachable=yes jexunit01b TpacallTests


if [ "X$1" != "X" ]; then
    jexunit01b $1|| exit 11

else
    jexunit01b Conversations || go_out 6
    jexunit01b TpForward || go_out 5
    jexunit01b TpcallTests || go_out 4
    jexunit01b TpacallTests || go_out 3
    jexunit01b ExceptionTests || go_out 2
fi

go_out 0
