#!/bin/bash
set -e

QLOG=Q.log
ULOG=U.log
QERR=Q.err
UERR=U.err

(timeout 5 ./Q2 -t 4 -n 3 fifinho > $QLOG 2> $QERR; echo $? >> $QERR) &
(timeout 5 ./U2 -t 2      fifinho > $ULOG 2> $UERR; echo $? >> $UERR) &
sleep 7
test/aval1.sh $ULOG $QLOG
test/eval-stderr.sh $UERR $QERR
