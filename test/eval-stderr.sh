#/! /bin/sh

UERR=$1
QERR=$2

Ucontent=`cat $UERR`
Qcontent=`cat $QERR`

if [ "$Ucontent" != "0" ]; then
    echo $UERR not fine: content is "$Ucontent"
    exit 1
fi

if [ "$Qcontent" != "0" ]; then
    echo $QERR not fine: content is "$Qcontent"
    exit 1
fi
