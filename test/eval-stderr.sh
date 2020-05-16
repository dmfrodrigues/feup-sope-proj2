#/! /bin/sh

UERR=$1
QERR=$2

if [ -s $UERR ]; then
    echo $UERR not empty
    exit 1
fi

if [ -s $QERR ]; then
    echo $QERR not empty
    exit 1
fi
