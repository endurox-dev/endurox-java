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


export FIELDTBLS=Exfields,test.fd
export FLDTBLDIR=`pwd`

source ~/ndrx_home
unset NDRX_DEBUG_CONF
./jexunit00b AtmiCtxTest || exit 1
./jexunit00b BaddTest || exit 2

