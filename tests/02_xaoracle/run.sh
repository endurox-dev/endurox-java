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
export NDRX_SILENT=Y

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
    xadmin killall xmemck
    xadmin down -y 2>/dev/null

    exit $1
}

echo "Prepare the DB1_NAT mode... / JDBC java"
export NDRX_CCTAG=DB1_NAT/DEBUG
export NDRX_CCTAG_TEST=DB1_JDBC/DEBUG
xadmin down -y
rm $NDRX_APPHOME/log/*
xadmin start -y

echo "Running jexunit02b..."
NDRX_CCTAG=DB1_JDBC/DEBUG jexunit02b XAOraTests > $NDRX_APPHOME/log/jexunit02b.log 2>&1

RET=$?

if [ "X$RET" != "X0" ]; then
	echo "jexunit02b failed (DB1_JDBC)"
	go_out 1
fi
xadmin stop -y

#
# This is used by ndrxconfig.xml

echo "Prepare the JDBC mode..."
export NDRX_CCTAG=DB1_JDBC/DEBUG
export NDRX_CCTAG_TEST=DB1_JDBC/DEBUG
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

echo "Prepare the DB1_OCI mode..."

export NDRX_CCTAG=DB1_OCI/DEBUG
export NDRX_CCTAG_TEST=DB1_OCI/DEBUG
rm $NDRX_APPHOME/log/*
xadmin start -y

echo "Running jexunit02b..."
jexunit02b XAOraTests > $NDRX_APPHOME/log/jexunit02b.log 2>&1

RET=$?

if [ "X$RET" != "X0" ]; then
	echo "jexunit02b failed (DB1_JDBC)"
	go_out 1
fi

go_out 0
