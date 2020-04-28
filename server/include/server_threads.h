#ifndef SERVER_THREADS_H_INCLUDED
#define SERVER_THREADS_H_INCLUDED

#include "message.h"

/**
 * @brief Initialize server threads stuff.
 * 
 * Allows to keep a count on how many threads are running.
 * 
 * @return int EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int server_threads_init(void);

/**
 * @brief Cleanup server threads stuff.
 * 
 * @return int EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int server_threads_clean(void);

/**
 * @brief Create new thread to process a message.
 * 
 * Makes a copy of m, so you can free m afterwards.
 * 
 * @param m     Pointer to message to be processed
 * @return int  EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int server_create_thread(const message_t *m);

/**
 * @brief Wait for all threads to finish.
 * 
 * @return int EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int server_wait_all_threads(void);

#endif //SERVER_THREADS_H_INCLUDED
