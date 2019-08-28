#!/bin/bash

#
# @(#) Run the test case for Oracle Database Two phase commit processing
#  this is leak test runs for some time while xmemck checks the memory usage
#  statistics.
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
# Verify that we finish ok with out leaks...
#
function test_leak {

    echo "Scanning for leaks... $NDRXJ_LEAKTEST_NAME (sleep 5 for plot results...)"
    sleep 5

    LEAKS=`grep "Process leaky" log/XMEMCK.log`

    echo "leaks=[$LEAKS]"

    if [[ "X$LEAKS" != "X" ]]; then
            echo "Memory leaks detected for $NDRXJ_LEAKTEST_NAME!"
            go_out 99
    fi

}

export NDRXJ_LEAKTEST=200
export NDRXJ_LEAKTEST_NAME="basicXA"

echo "Prepare the DB1_NAT mode..."
export NDRX_CCTAG=DB1_NAT/DEBUGOFF
export NDRX_CCTAG_TEST=DB1_JDBC/DEBUGOFF
xadmin down -y
rm $NDRX_APPHOME/log/*

NDRX_CCTAG=DEBUG xmemck -d45 -s50 -t90 -m jexunit02b -m jserver02b -m tmsrv &

xadmin start -y

echo "Running jexunit02b..."
NDRX_CCTAG=DB1_JDBC/DEBUGOFF jexunit02b XAOraTests > $NDRX_APPHOME/log/jexunit02b.log 2>&1

RET=$?

if [ "X$RET" != "X0" ]; then
	echo "jexunit02b failed (DB1_JDBC)"
	go_out 1
fi
xadmin stop -y
test_leak;

#
# This is used by ndrxconfig.xml

echo "Prepare the JDBC mode..."
export NDRX_CCTAG=DB1_JDBC/DEBUGOFF
export NDRX_CCTAG_TEST=DB1_JDBC/DEBUGOFF
rm $NDRX_APPHOME/log/*
xadmin start -y

echo "Running jexunit02b..."
jexunit02b XAOraTests > $NDRX_APPHOME/log/jexunit02b.log 2>&1

RET=$?

if [ "X$RET" != "X0" ]; then
	echo "jexunit02b failed (DB1_JDBC)"
	go_out 1
fi
xadmin stop -y
test_leak;

echo "Prepare the DB1_OCI mode..."

export NDRX_CCTAG=DB1_OCI/DEBUGOFF
export NDRX_CCTAG_TEST=DB1_OCI/DEBUGOFF
rm $NDRX_APPHOME/log/*
xadmin start -y

echo "Running jexunit02b..."
jexunit02b XAOraTests > $NDRX_APPHOME/log/jexunit02b.log 2>&1

RET=$?

if [ "X$RET" != "X0" ]; then
	echo "jexunit02b failed (DB1_JDBC)"
	go_out 1
fi

xadmin stop -y
test_leak;

go_out 0
