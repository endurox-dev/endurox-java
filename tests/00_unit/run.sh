#!/bin/bash

#
# @(#) Run the test case
#


export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../../libsrc/c
java -cp ../../libsrc/enduroxjava.jar:../libs/junit-4.12.jar:../libs/hamcrest-core-1.3.jar:./jexunit00.jar org.junit.runner.JUnitCore AtmiCtxTest

