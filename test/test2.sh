#!/bin/bash
set -e

QLOG=Q.log
ULOG=U.log
QERR=Q.err
UERR=U.err

timeout 16 ./Q1 -t 15 fifinho > $QLOG 2> $QERR &
timeout 21 ./U1 -t 20 fifinho > $ULOG 2> $UERR &
sleep 21
test/aval1.sh $ULOG $QLOG
