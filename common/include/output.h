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

/**
 * @brief Output logs.
 * 
 * @param m     Message to print
 * @param op    Operation to be registered
 * @return int  EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int output(const message_t *m, operation_t op);

#endif
