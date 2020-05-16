#!/bin/bash
set -e

QLOG=Q.log
ULOG=U.log
QERR=Q.err
UERR=U.err

(timeout 5 ./Q2 -t 4 -n 2 -l 1 fifinho > $QLOG 2> $QERR; echo $? >> $QERR) &
(timeout 5 ./U2 -t 3           fifinho > $ULOG 2> $UERR; echo $? >> $UERR) &
sleep 6
test/aval1.sh $ULOG $QLOG
test/eval-stderr.sh $UERR $QERR
