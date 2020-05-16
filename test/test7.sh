#!/bin/bash
set -e

QLOG=Q.log
ULOG=U.log
QERR=Q.err
UERR=U.err

(timeout 4 ./Q2 -t 2 -n 1 -l 5 fifinho > $QLOG 2> $QERR; echo $? >> $QERR) &
(timeout 3 ./U2 -t 1           fifinho > $ULOG 2> $UERR; echo $? >> $UERR) &
sleep 5
test/aval1.sh $ULOG $QLOG
test/eval-stderr.sh $UERR $QERR
