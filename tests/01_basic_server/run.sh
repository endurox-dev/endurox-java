#!/bin/bash

#
# @(#) Run the test case
#

pushd .
cd conf
. setndrx
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

jexunit01b TpcallTests || go_out 2

go_out 0
