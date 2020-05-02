#!/bin/bash

GREEN_INTENSIVE='\033[1;32m'
RED_INTENSIVE='\033[1;31m'
NC_INTENSIVE='\033[1m' # No Color
NC='\033[0m' # No Color

CLOG=U.log
SLOG=Q.log
CERR=U.err
SERR=Q.err
SPROG=./Q1
CPROG=./U1
FIFONAME=fifo

test_init () {
    rm -f $CLOG $CERR $SLOG $SERR
    s="${NC_INTENSIVE}Test '$1'...${NC}"
    printf %-40b "$s"
}

test_print_outcome() {
    if [[ $1 -eq 0 ]] ; then
        printf "[ ${GREEN_INTENSIVE}OK${NC} ]\n"
        return 0
    else
        printf "[ ${RED_INTENSIVE}FAILED${NC} ]\n"
        return 1
    fi
}

test_assert_general () {
    nIWANT=`grep IWANT $CLOG | wc -l`
    nRECVD=`grep RECVD $SLOG | wc -l`
    nENTER=`grep ENTER $SLOG | wc -l`
    nIAMIN=`grep IAMIN $CLOG | wc -l`
    nTIMUP=`grep TIMUP $SLOG | wc -l`
    n2LATE=`grep 2LATE $SLOG | wc -l`
    nCLOSD=`grep CLOSD $CLOG | wc -l`
    nFAILD=`grep FAILD $CLOG | wc -l`
    nGAVUP=`grep GAVUP $SLOG | wc -l`

    # if [ "$nIWANT" != "$(($nRECVD+1))" ]; then return 1; fi

    # if [ "$nRECVD" != "$nENTER" ]; then return 1; fi

    # if [ "$nENTER" >= "$nTIMUP" ]; then return 1; fi

    # if [ "$nIAMIN" != "$nTIMUP" ]; then return 1; fi

    # if [ "$n2LATE" != "$nCLOSD" ]; then return 1; fi

    # if [ "$nCLOSD" != "1" ]; then return 1; fi

    # if [ "$nGAVUP" != "0" ]; then return 1; fi

}

test_assert_notend () {

    test_assert_general
    if [ "$?" != "0" ] ; then return 1; fi

    nIWANT=`grep IWANT $CLOG | wc -l`
    nRECVD=`grep RECVD $SLOG | wc -l`
    nENTER=`grep ENTER $SLOG | wc -l`
    nIAMIN=`grep IAMIN $CLOG | wc -l`
    nTIMUP=`grep TIMUP $SLOG | wc -l`
    n2LATE=`grep 2LATE $SLOG | wc -l`
    nCLOSD=`grep CLOSD $CLOG | wc -l`
    nFAILD=`grep FAILD $CLOG | wc -l`
    nGAVUP=`grep GAVUP $SLOG | wc -l`



    if [ "$n2LATE" != "0" ]; then return 1; fi
}

test_assert_end () {
    test_assert_general
    if [ "$?" != "0" ] ; then return 1; fi
}

test_notend1 () {

    timeout 5 $SPROG -t 4 $FIFONAME >  $SLOG 2> $SERR &
    timeout 2 $CPROG -t 1 $FIFONAME >> $CLOG 2>> $CERR
    if [ "$?" != "0" ] ; then return 1; fi
    timeout 2 $CPROG -t 1 $FIFONAME >> $CLOG 2>> $CERR
    if [ "$?" != "0" ] ; then return 1; fi
    sleep 2

    test_assert_notend
    if [ "$?" != "0" ] ; then return 1; fi

}

test_end1 () {
    ret=0
    timeout 5 $SPROG -t 4 $FIFONAME >  $SLOG 2> $SERR &
    timeout 4 $CPROG -t 3 $FIFONAME >> $CLOG 2>> $CERR
    if [ "$?" != "0" ] ; then return 1; fi
    timeout 4 $CPROG -t 3 $FIFONAME >> $CLOG 2>> $CERR
    if [ "$?" != "0" ] ; then return 1; fi
    test_assert_end
    if [ "$?" != "0" ] ; then return 1; fi
}

test_client1 () {
    timeout 3 $CPROG -t 2 $FIFONAME >  $SLOG 2> $SERR
    if [ "$?" != "0" ] ; then return 1; fi
}

test_run() {
    test_init "$1"
    $1
    ret="$?"
    test_print_outcome "$ret"
    if [ "$ret" != "0" ]; then exit 1; fi
    return $ret
}

test_run "test_end1"
test_run "test_notend1"
test_run "test_client1"

