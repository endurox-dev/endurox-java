#!/bin/bash

#
# @(#) Run the test case
#


export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../../libsrc/c
java -cp ../../libsrc/enduroxjava.jar:../libs/junit.jar:../libs/hamcrest-core.jar:./jexunit00.jar org.junit.runner.JUnitCore AtmiCtxTest

