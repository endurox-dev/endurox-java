#!/bin/bash

#
# @(#) Run the test case
#

pushd .
cd conf
. setndrx
popd

# Start the enduro/x app server (which will boot the our server executable)

xadmin start -y
rm /tmp/01_CLIENT.log 2>/dev/null

# should print some hello world
client.sh > /tmp/01_CLIENT.log 2>&1

RET=$?

# shutdown the app server
xadmin stop -c -y

exit $?
