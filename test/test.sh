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
    nFAILD=`grep FAILD $CLOG | wc -l`; echo $nFAILD
    nGAVUP=`grep GAVUP $SLOG | wc -l`; echo $nGAVUP



    if [ "$nIWANT" != "$nRECVD" ]; then return 1; fi

    if [ "$nRECVD" != "$(($nENTER + $n2LATE))" ]; then return 1; fi

    if [ "$nENTER" != "$nTIMUP" ]; then return 1; fi

    if [ "$nIAMIN" != "$nTIMUP" ]; then return 1; fi

    if [ "$n2LATE" != "$nCLOSD" ]; then return 1; fi
    echo L52
    if [ "$nFAILD" != "0" ]; then return 1; fi
    echo L54
    if [ "$nGAVUP" != "0" ]; then return 1; fi
    echo L56
}

test_assert_notend () {

    test_assert_general

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
}

test_notend1 () {
    test_init "notend1"

    timeout 5 $SPROG -t 4 $FIFONAME >  $SLOG 2> $SERR &
    timeout 2 $CPROG -t 1 $FIFONAME >> $CLOG 2>> $CERR
    timeout 2 $CPROG -t 1 $FIFONAME >> $CLOG 2>> $CERR
    sleep 2

    test_assert_notend
    ret=$?
    test_print_outcome "$ret"
    return $ret
}

test_end1 () {
    test_init "end1"

    timeout 5 $SPROG -t 4 $FIFONAME >  $SLOG 2> $SERR &
    timeout 4 $CPROG -t 3 $FIFONAME >> $CLOG 2>> $CERR
    timeout 4 $CPROG -t 3 $FIFONAME >> $CLOG 2>> $CERR

    test_assert_end
    ret=$?
    test_print_outcome "$ret"
    return $ret
}

test_client1 () {
    test_init "client1"

    timeout 3 $CPROG -t 2 $FIFONAME >  $SLOG 2> $SERR

    ret=$?
    test_print_outcome "$ret"
    return $ret
}

# test_notend1 || exit 1;
# test_end1    || exit 1;
test_client1 || exit 1;
