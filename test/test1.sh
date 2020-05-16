#!/bin/bash
set -e

QLOG=Q.log
ULOG=U.log
QERR=Q.err
UERR=U.err

(timeout 11 ./Q2 -t 10 fifinho > $QLOG 2> $QERR; echo $? >> $QERR) &
(timeout  8 ./U2 -t  7 fifinho > $ULOG 2> $UERR; echo $? >> $UERR) &
sleep 12
test/aval1.sh $ULOG $QLOG
test/eval-stderr.sh $UERR $QERR
