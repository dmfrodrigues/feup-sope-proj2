#!/bin/bash
set -e

QLOG=Q.log
ULOG=U.log
QERR=Q.err
UERR=U.err

timeout 2 ./Q1 -t 1 fifinho > $QLOG 2> $QERR &
timeout 3 ./U1 -t 2 fifinho > $ULOG 2> $UERR &
sleep 4
test/aval1.sh $ULOG $QLOG
