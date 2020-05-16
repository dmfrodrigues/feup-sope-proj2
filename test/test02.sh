#!/bin/bash
set -e

QLOG=Q.log
ULOG=U.log
QERR=Q.err
UERR=U.err

(timeout 16 ./Q2 -t 15 fifinho > $QLOG 2> $QERR; echo $? >> $QERR) &
(timeout 21 ./U2 -t 20 fifinho > $ULOG 2> $UERR; echo $? >> $UERR) &
sleep 21
test/aval1.sh $ULOG $QLOG
test/eval-stderr.sh $UERR $QERR
