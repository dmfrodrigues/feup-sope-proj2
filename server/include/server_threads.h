#ifndef SERVER_THREADS_H_INCLUDED
#define SERVER_THREADS_H_INCLUDED

#include "message.h"
#include <semaphore.h>

sem_t s;

/**
 * @brief Initialize server threads stuff.
 * 
 * Allows to keep a count on how many threads are running.
 * 
 * @return int EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int server_threads_init(int nplaces, int nthreads);

/**
 * @brief Cleanup server threads stuff.
 * 
 * @return int EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int server_threads_clean(void);

/**
 * @brief Attempts to enter bathroom, creating a new server thread in the process.
 * 
 * @param m     Pointer to message to be processed
 * @return int  EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int try_entering(message_t *m_);

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

/**
 * @brief Clears public FIFO "buffer" and closes server
 * 
 * @param fifoname public FIFO
 * @return int EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int server_close_service(char* fifoname);

#endif //SERVER_THREADS_H_INCLUDED
