#!/bin/bash

#
# @(#) Run the test case for Oracle Database Two phase commit processing
#

#
# Ask for JMV path.., if needed
#

ADDLIBPATH=""

#
# Load env
#
source ~/ndrx_home

#
# Have some runtime 
#
xadmin provision -d -vaddubf=test.fd \
	-vshLibs=`pwd`/../../libsrc/c:`pwd`/../../libexjlds$ADDLIBPATH\
	-vqprefix=test2

export ASAN_OPTIONS=handle_segv=0

pushd .
cd conf
. settest2
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
rm /tmp/02_CLIENT.log 2>/dev/null

#
# Running client 01
#
echo "Running jclient02b..."
jclient02b > /tmp/02_CLIENT.log 2>&1

RET=$?

if [ "X$RET" != "X0" ]; then
	echo "jclient02b failed"
	go_out 1
fi

#
# Running unit tests...
#
xadmin psc


if [ "X$1" != "X" ]; then

else

fi

go_out 0
