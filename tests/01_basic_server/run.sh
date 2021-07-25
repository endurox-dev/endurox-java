#!/bin/bash

#
# @(#) Run the test case
#

#
# Ask for JMV path.., if needed
#

ADDLIBPATH=""

source ~/ndrx_home

# prepare test dir..
rm -rf QTEST 2>/dev/null
mkdir QTEST

#
# Have some runtime 
#
xadmin provision -d -vaddubf=test.fd -vviewInstall=y -vviewFiles=jview01.V \
	-vshLibs=`pwd`/../../libsrc/c:`pwd`/../../libexjlds$ADDLIBPATH -vnodeid=2 -vinstallQ=n

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


#
# truncate log files before test..
#
function trunc_logs {
    pushd .
    cd $NDRX_APPHOME/log || exit 1
    for f in `ls *`; do
         echo > $f
    done
    popd
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
    trunc_logs;
    jexunit01b $1 || go_out 11

else
    trunc_logs;
    jexunit01b SrvThreads || go_out 12
    # start all the copies for high speed testing..
    echo "Before server boot"
    xadmin start -i 101
    xadmin start -i 102
    xadmin start -i 103
    xadmin start -i 104
    echo "After server boot"
    xadmin psc
    trunc_logs;
    jexunit01b CltThreads || go_out 10
    # back to single server
    xadmin stop -s jserver01_2b
    xadmin start -i 100
    trunc_logs;
    jexunit01b QueueTests || go_out 9
    trunc_logs;
    jexunit01b TpBroadcastTests || go_out 8
    trunc_logs;
    jexunit01b TpNotifyTests || go_out 7
    trunc_logs;
    jexunit01b Conversations || go_out 6
    trunc_logs;
    jexunit01b TpForward || go_out 5
    trunc_logs;
    jexunit01b TpcallTests || go_out 4
    trunc_logs;
    jexunit01b TpacallTests || go_out 3
    trunc_logs;
    jexunit01b ExceptionTests || go_out 2
fi

go_out 0
