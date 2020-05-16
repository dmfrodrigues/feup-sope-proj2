#ifndef CLIENT_TBDNAME_H_INCLUDED
#define CLIENT_TBDNAME_H_INCLUDED

#include "message.h"
#include "client_thread_args.h"

#include <sys/types.h>
#include <sys/stat.h>

/**
 * @brief Initialize client threads stuff.
 * 
 * Allows to keep a count on how many threads are running.
 * 
 * @return int EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int client_threads_init(void) __attribute__((warn_unused_result));

/**
 * @brief Cleanup client threads stuff.
 * 
 * @return int EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int client_threads_clear(void) __attribute__((warn_unused_result));

/**
 * @brief Function to be executed by a new thread.
 * 
 * @param arg Argument
 * @return void* Return value
 */
void *client_execute_thread(void *arg);

/**
 * @brief Wait for all threads to finish.
 * 
 * @return int EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int client_wait_all_threads(void) __attribute__((warn_unused_result));

#endif //CLIENT_TBDNAME_H_INCLUDED