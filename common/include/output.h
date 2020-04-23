#ifndef OUTPUT_H_INCLUDED
#define OUTPUT_H_INCLUDED

#include "message.h"

typedef enum {
    op_IWANT,
    op_RECVD,
    op_ENTER,
    op_IAMIN,
    op_TIMUP,
    op_2LATE,
    op_CLOSD,
    op_FAILD,
    op_GAVUP
} operation_t;

int output(const message_t *m, operation_t op);

#endif
