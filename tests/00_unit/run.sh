#!/bin/bash

#
# @(#) Run the test case
#


#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../../libsrc/c
#java -cp ../../libsrc/enduroxjava.jar:../libs/junit.jar:../libs/hamcrest-core.jar:./jexunit00.jar org.junit.runner.JUnitCore AtmiCtxTest

# Running with compiler;


#tests/00_unit/run.sh
#export LD_LIBRARY_PATH=/usr/lib/jvm/default-java/lib/:/usr/lib/jvm/default-java/lib/server/
#../../exjld/exjld -I ../../include -L ../../libexjlds -L/usr/lib/jvm/default-java/lib/ -L /usr/lib/jvm/default-java/lib/server/ -k -m org.junit.runner.JUnitCore ../../libsrc/enduroxjava.jar ../libs/junit.jar ../libs/hamcrest-core.jar ./jexunit00.jar

#export LD_LIBRARY_PATH=/usr/lib/jvm/default-java/lib/:/usr/lib/jvm/default-java/lib/server/
#export JAVA_HOME=/usr/lib/jvm/java-11-openjdk-amd64
#./a.out AtmiCtxTest


source ~/ndrx_home

# Create some test env...
# override some details to local...
#
xadmin provision -d -vaddubf=test.fd
pushd . 

cd conf
. settest1

popd

# Use decimal sepertar with dots..
export LANG=en_UK.UTF-8
export NDRX_CCONFIG1=`pwd`/settings.ini
export FIELDTBLS=Exfields,test.fd
export FLDTBLDIR=`pwd`
unset NDRX_DEBUG_CONF

#
# if we have some command line arg, the run particular class
# otherwise we run all
#

if [ "X$1" != "X" ]; then

    ./jexunit00b $1|| exit 11

else

    ./jexunit00b AtmiCtxTest || exit 1
    ./jexunit00b BaddTest || exit 2
    ./jexunit00b BaddfastTest || exit 3
    ./jexunit00b BchgTest || exit 4
    #valgrind --leak-check=full ./jexunit00b BboolTest || exit 5
    ./jexunit00b BboolTest || exit 5
    ./jexunit00b BDelTest || exit 6
    ./jexunit00b BprintTest || exit 7
    ./jexunit00b ButilTest > ButilTest.log 2>&1 || exit 8

    TESTOUT=`grep "HELLO MARS" ButilTest.log`
    echo "GOT OUT: [$TESTOUT]"

    if [[ "X$TESTOUT" == "X" ]]; then

        echo "ButilTest failed -> logging not working"
        exit 8
    fi

    ./jexunit00b BProjTest || exit 9
    ./jexunit00b BMarshalTest|| exit 10
    ./jexunit00b BjsonTest|| exit 11
    ./jexunit00b TypedStringTest|| exit 12
    ./jexunit00b AtmiCtxGetBuffers|| exit 13
    ./jexunit00b TypedJsonTest|| exit 14
    ./jexunit00b TypedCarrayTest|| exit 15
    ./jexunit00b TypedBufferTest || exit 16

fi

exit 0
