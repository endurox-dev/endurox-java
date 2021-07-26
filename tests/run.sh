#!/bin/bash

#
# @(#) Integration tests. This will load ~/ndrx_home, in order to check the
#  database avaliblity
#

source ~/ndrx_home
# remove any user resources...
xadmin udown -y
echo "Oracle [$EX_ORA_HOST] Postgres [$EX_PG_HOST]"

> ./test.out
# Have some terminal output...
tail -f test.out &

(

M_tests=0
M_ok=0
M_fail=0
M_last_stat=0
run_test () {

        test=$1
	runner=$2
        M_tests=$((M_tests + 1))
        echo "*** RUNNING [$test] [$runner]"

        pushd .
        cd $test
        #./run.sh
	$runner
        ret=$?
        popd
        
        echo "*** RESULT [$test] [$runner] $ret"
        
        if [[ $ret -eq 0 ]]; then
                M_ok=$((M_ok + 1))
		M_last_stat=0
        else
                M_fail=$((M_fail + 1))
		M_last_stat=1
        fi
}

run_test "00_unit" "./run.sh"
run_test "01_basic_server" "./run.sh"

if [ "X$M_last_stat" == "X0" ]; then
	echo "Base test ok - run leak test"
	run_test "01_basic_server" "./run-leak.sh"
fi

# Oracle tests, if configured (see module_developer_guide for details)
if [ "X$EX_ORA_HOST" != "X" ]; then
	run_test "02_xaoracle" "./run.sh"

	if [ "X$M_last_stat" == "X0" ]; then
		echo "Base test ok - run leak test"
		run_test "02_xaoracle" "./run-leak.sh"
	fi
fi

# Postgres tests, if configured (see module_developer_guide for details)
if [ "X$EX_PG_HOST" != "X" ]; then
	run_test "03_xapostgres" "./run.sh"

	if [ "X$M_last_stat" == "X0" ]; then
		run_test "03_xapostgres" "./run-leak.sh"
	fi
fi

grep RESULT test.out
echo "*** SUMMARY $M_tests tests executed. $M_ok passes, $M_fail failures"

xadmin killall tail

exit $M_fail
) > test.out 2>&1

