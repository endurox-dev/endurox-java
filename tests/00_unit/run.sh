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

export NDRX_CCONFIG=`pwd`/settings.ini
export FIELDTBLS=Exfields,test.fd
export FLDTBLDIR=`pwd`
unset NDRX_DEBUG_CONF
#./jexunit00b AtmiCtxTest || exit 1
#./jexunit00b BaddTest || exit 2
#./jexunit00b BaddfastTest || exit 3
#./jexunit00b BchgTest || exit 4
#valgrind --leak-check=full ./jexunit00b BboolTest || exit 5
#./jexunit00b BboolTest || exit 5
./jexunit00b BDelTest || exit 6

