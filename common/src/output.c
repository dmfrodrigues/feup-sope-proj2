#include "output.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1024

int output(const message_t *m, operation_t op){
    const char *op_str = NULL;
    switch(op){
        case op_IWANT: op_str = "IWANT"; break;
        case op_RECVD: op_str = "RECVD"; break;
        case op_ENTER: op_str = "ENTER"; break;
        case op_IAMIN: op_str = "IAMIN"; break;
        case op_TIMUP: op_str = "TIMUP"; break;
        case op_2LATE: op_str = "2LATE"; break;
        case op_CLOSD: op_str = "CLOSD"; break;
        case op_FAILD: op_str = "FAILD"; break;
        case op_GAVUP: op_str = "GAVUP"; break;
        default: return EXIT_FAILURE;
    }

    char buf[BUF_SIZE];
    if(sprintf(buf, "%ld ; %d ; %d ; %lu ; %d ; %d ; %s\n", time(NULL), m->i, m->pid, m->tid, m->dur, m->pl, op_str) < 0) return EXIT_FAILURE;
    if(write(STDOUT_FILENO, buf, strlen(buf)) == -1) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}
