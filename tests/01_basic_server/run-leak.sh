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
xadmin down -y 2>/dev/null

exit $1
}

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

export NDRXJ_LEAKTEST=60

echo "Test period $NDRXJ_LEAKTEST sec"
# Valgrind debug:
#  valgrind --main-stacksize=9916777216 --leak-check=full --show-leak-kinds=all --show-reachable=yes jexunit01b TpacallTests
#jexunit01b TpcallTests || go_out 2

#
# Start Enduro/X leak monitor...
#
xmemck -m jexunit01b -m jserver01_2b &

jexunit01b TpacallTests || go_out 3

go_out 0
