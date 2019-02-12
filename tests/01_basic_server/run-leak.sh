#!/bin/bash

#
# @(#) Run the test case, leak tests
#

pushd .
cd conf
. setndrx
popd

export ASAN_OPTIONS=handle_segv=0

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

	LEAKS=`grep "LEAK" log/XMEMCK`

	echo "leaks=[$LEAKS]"

	if [[ "X$LEAKS" != "X" ]]; then
		echo "Memory leaks detected for $NDRXJ_LEAKTEST_NAME!"
		go_out 99
	fi

}

echo "debug off"
export NDRX_CCTAG="off"

xadmin start -y
rm log/* 2>/dev/null

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
xmemck -s30 -t60 -m jexunit01b -m jserver01_2b &


export NDRXJ_LEAKTEST_NAME="tpcallNullRsp"
jexunit01b TpcallTests || go_out 7
tmshutdown -y; tmboot -y
test_leak;

exit 0

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

