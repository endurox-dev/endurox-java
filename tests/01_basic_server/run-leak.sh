#!/bin/bash

#
# @(#) Run the test case, leak tests
#

#
# Ask for JMV path.., if needed
#

ADDLIBPATH=""

# Load env
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
    xadmin killall xmemck
    xadmin down -y 2>/dev/null

    exit $1
}


#
# Verify that we finish ok with out leaks...
#
function test_leak {

    echo "Scanning for leaks... $NDRXJ_LEAKTEST_NAME (sleep 5 for plot results...)"
    sleep 5

    LEAKS=`grep "Process leaky" log/XMEMCK`

    echo "leaks=[$LEAKS]"

    if [[ "X$LEAKS" != "X" ]]; then
            echo "Memory leaks detected for $NDRXJ_LEAKTEST_NAME!"
            go_out 99
    fi

}

echo "debug off"
export NDRX_CCTAG="off"

rm log/* 2>/dev/null
xadmin start -y
xadmin psc

#
# Running client 01
#
echo "Running jclient01b..."
jclient01b > log/01_CLIENT.log 2>&1

RET=$?

if [ "X$RET" != "X0" ]; then
echo "jclient01b failed"
go_out 1
fi

#
# Running unit tests...
#

export NDRXJ_LEAKTEST=200

echo "Test period $NDRXJ_LEAKTEST sec"
# Valgrind debug:
#  valgrind --main-stacksize=9916777216 --leak-check=full --show-leak-kinds=all --show-reachable=yes jexunit01b TpacallTests
#jexunit01b TpcallTests || go_out 2

#
# Start Enduro/X leak monitor...
#
xmemck -d45 -s50 -t90 -m jexunit01b -m jserver01_2b &

export NDRXJ_LEAKTEST_NAME="srvThreads"
jexunit01b SrvThreads || go_out 15
tmshutdown -y; tmboot -y
test_leak;

# start all the copies for high speed testing..
export NDRXJ_LEAKTEST_NAME="clThreadsTest"
echo "Before server boot"
xadmin start -i 101
xadmin start -i 102
xadmin start -i 103
xadmin start -i 104
echo "After server boot"
xadmin psc
jexunit01b CltThreads || go_out 10
# back to single server
xadmin stop -s jserver01_2b
xadmin start -i 100


export NDRXJ_LEAKTEST_NAME="basicQTest"
jexunit01b QueueTests || go_out 14
tmshutdown -y; tmboot -y
test_leak;


export NDRXJ_LEAKTEST_NAME="basicQTest"
jexunit01b QueueTests || go_out 14
tmshutdown -y; tmboot -y
test_leak;

export NDRXJ_LEAKTEST_NAME="tpbroadcastTest"
jexunit01b TpBroadcastTests || go_out 13
tmshutdown -y; tmboot -y
test_leak;

export NDRXJ_LEAKTEST_NAME="tpnotifyTest"
jexunit01b TpNotifyTests || go_out 12
tmshutdown -y; tmboot -y
test_leak;

export NDRXJ_LEAKTEST_NAME="tpcancelTest"
jexunit01b TpacallTests || go_out 11
tmshutdown -y; tmboot -y
test_leak;

export NDRXJ_LEAKTEST_NAME="convTest"
jexunit01b Conversations || go_out 10
tmshutdown -y; tmboot -y
test_leak;

export NDRXJ_LEAKTEST_NAME="bufferCrossTestAsync"
jexunit01b TpacallTests || go_out 9
tmshutdown -y; tmboot -y
test_leak;

export NDRXJ_LEAKTEST_NAME="bufferCrossTest"
jexunit01b TpcallTests || go_out 8
tmshutdown -y; tmboot -y
test_leak;

export NDRXJ_LEAKTEST_NAME="tpcallNullRsp"
jexunit01b TpcallTests || go_out 7
tmshutdown -y; tmboot -y
test_leak;

export NDRXJ_LEAKTEST_NAME="tpcallNullNull"
jexunit01b TpcallTests || go_out 6
tmshutdown -y; tmboot -y
test_leak;

export NDRXJ_LEAKTEST_NAME="tpcallNullCallTest"
jexunit01b TpcallTests || go_out 5
tmshutdown -y; tmboot -y
test_leak;

# needs to export exact test case name, via hash # does not work case selection...
# needed due to multiple scenarios and we need to understand exactly when we got a leak
export NDRXJ_LEAKTEST_NAME="tpcallTest"
jexunit01b TpcallTests || go_out 4
tmshutdown -y; tmboot -y
test_leak;

# needs to export exact test case name, via hash # does not work case selection...
export NDRXJ_LEAKTEST_NAME="tpacallTest"
jexunit01b TpacallTests || go_out 3
tmshutdown -y; tmboot -y
test_leak;

go_out 0

