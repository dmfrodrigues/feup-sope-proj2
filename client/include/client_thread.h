#ifndef CLIENT_TBDNAME_H_INCLUDED
#define CLIENT_TBDNAME_H_INCLUDED

#include "message.h"
#include "client_thread_args.h"

#include <sys/types.h>
#include <sys/stat.h>

/**
 * @brief Function used by the previously created thread
 * 
 * @param arg 
 * @return void* 
 */
void *client_execute_thread(void *arg);

#endif //CLIENT_TBDNAME_H_INCLUDED