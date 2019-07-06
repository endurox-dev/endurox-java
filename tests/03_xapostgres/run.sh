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
	-vqprefix=test3

export ASAN_OPTIONS=handle_segv=0

pushd .
cd conf
. settest3
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

rm $NDRX_APPHOME/log/*.log


#
# First test with JDBC TMSRV
#
echo "Running JDBC TMSRV Mode"
xadmin start -i 40
xadmin start -i 340

#
# Running client 03
#
echo "Running jclient03b..."
NDRX_CCTAG=DB1_JDBC/DEBUG jexunit03b XAPgTests > $NDRX_APPHOME/log/jexunit03b.log 2>&1

RET=$?

if [ "X$RET" != "X0" ]; then
	echo "jexunit03b failed"
	go_out 1
fi

#
# Run with PQ TMSRV
#
xadmin stop -y
rm *.log

xadmin start -i 140
xadmin start -i 340


#
# Running client 03
#
echo "Running jclient03b... 2"
NDRX_CCTAG=DB1_JDBC/DEBUG jexunit03b XAPgTests > $NDRX_APPHOME/log/jexunit03b.log 2>&1

RET=$?

if [ "X$RET" != "X0" ]; then
	echo "jexunit03b failed 2"
	go_out 1
fi


go_out 0
