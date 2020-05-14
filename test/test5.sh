#!/bin/bash
set -e

QLOG=Q.log
ULOG=U.log
QERR=Q.err
UERR=U.err

timeout 3 ./Q2 -t 2 -n 1 -l 5 fifinho > $QLOG 2> $QERR &
timeout 2 ./U2 -t 1      fifinho > $ULOG 2> $UERR &
sleep 4
test/aval1.sh $ULOG $QLOG
