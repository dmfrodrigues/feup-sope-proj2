#ifndef SERVER_THREADS_H_INCLUDED
#define SERVER_THREADS_H_INCLUDED

#include "message.h"
#include <semaphore.h>

sem_t thread_semaphore;
sem_t place_semaphore;

/**
 * @brief Initialize server threads stuff.
 * 
 * Allows to keep a count on how many threads are running.
 * 
 * @return int EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int server_threads_init(int nplaces, int nthreads) __attribute__((warn_unused_result));

/**
 * @brief Cleanup server threads stuff.
 * 
 * @return int EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int server_threads_clean(void) __attribute__((warn_unused_result));

/**
 * @brief Create new thread to process a message.
 * 
 * Makes a copy of m, so you can free m afterwards.
 * 
 * @param m     Pointer to message to be processed
 * @return int  EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int server_create_thread(const message_t *m) __attribute__((warn_unused_result));

/**
 * @brief Wait for all threads to finish.
 * 
 * @return int EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int server_wait_all_threads(void) __attribute__((warn_unused_result));

/**
 * @brief Clears public FIFO "buffer" and closes server
 * 
 * @param public_fifo_path public FIFO
 * @return int EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int server_close_service(const char *public_fifo_path) __attribute__((warn_unused_result));

#endif //SERVER_THREADS_H_INCLUDED
