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

#
# This is used by ndrxconfig.xml
#
export NDRX_DEBUGOUT=DEBUG

xadmin start -y
rm /tmp/02_CLIENT.log 2>/dev/null


echo "Prepare the JDBC mode..."
xadmin stop -s tmsrv
xadmin start -i 40


echo "Running jexunit02b..."
NDRX_CCTAG=DB1_JDBC/DEBUG jexunit02b XAOraTests > $NDRX_APPHOME/log/jexunit02b.log 2>&1

RET=$?

if [ "X$RET" != "X0" ]; then
	echo "jexunit02b failed (DB1_JDBC)"
	go_out 1
fi

echo "Prepare the DB1_OCI mode..."
xadmin stop -s tmsrv
xadmin start -i 140

echo "Running jexunit02b..."
NDRX_CCTAG=DB1_JDBC/DEBUG jexunit02b XAOraTests >> $NDRX_APPHOME/log/jexunit02b.log 2>&1

RET=$?

if [ "X$RET" != "X0" ]; then
	echo "jexunit02b failed (DB1_JDBC)"
	go_out 1
fi

echo "Prepare the DB1_NAT mode..."
xadmin stop -s tmsrv
xadmin start -i 240

NDRX_CCTAG=DB1_NAT/DEBUG jexunit02b XAOraTests >> $NDRX_APPHOME/log/jexunit02b.log 2>&1

RET=$?

if [ "X$RET" != "X0" ]; then
	echo "jexunit02b failed (DB1_NAT)"
	go_out 1
fi

go_out 0
