#ifndef CLIENT_TBDNAME_H_INCLUDED
#define CLIENT_TBDNAME_H_INCLUDED

#include "message.h"

#include <sys/types.h>
#include <sys/stat.h>

#define NANOSECOND_MULT     1000000 // from ms to ns
#define MILLISECOND_MULT    1000    // from sec to ms

#define REQUEST_PL_FIELD    -1      // message_t pl field for request

typedef struct {

    pthread_t *tid;

    int *request_number;

    char *public_fifoname;

} t_args;


/**
 * @brief Generates a random number of ms between the lower and upper limits
 * 
 * @param lower 
 * @param upper 
 * @return long 
 */
long client_generate_random_ms(int lower, int upper); // use default values: lower = 0, upper = 10

/**
 * @brief Opens public fifo previously created by the server for writing
 * 
 * @param pathname      fifo pathname
 * @param fd            file descriptor
 * @return int          EXIT_FAILURE if error, EXIT_SUCCESS otherwise
 */
int client_open_public_fifo(const char *pathname, int *fd);


int client_create_private_fifo(mode_t mode, pid_t pid, pthread_t tid, char *pathname);

/**
 * @brief Opens (private) fifo for reading
 * 
 * @param pathname 
 * @param fd            "returns" via this parameter
 * @return int          EXIT_FAILURE if error, EXIT_SUCCESS otherwise
 */
int client_open_private_fifo(const char *pathname, int *fd);

/**
 * @brief Closes file descriptor and destroys fifo (unlink)
 * 
 * @param pathname 
 * @param fd            file descriptor to close
 * @return int          EXIT_FAILURE if error, EXIT_SUCCESS otherwise
 */
int client_elim_private_fifo(const char *pathname, int *fd);

/**
 * @brief Generates a request with random arg (access duration)
 * 
 * @param tid               thread id (client thread in this case)
 * @param request_number    
 * @return message_t        request to be sent to server
 */
message_t client_generate_rand_request(pthread_t tid, int *request_number);

/**
 * @brief Creates a new thread (request) that is then sent to server
 * 
 * @param request_number 
 * @param fifoname          name of channel to send the request
 * @return int 
 */
int client_create_thread(int *request_number, char *fifoname);

/**
 * @brief Function used by the previously created thread
 * 
 * @param arg 
 * @return void* 
 */
void *client_execute_thread(void *arg);

/**
 * @brief Sends request via the public fifo to the server
 * 
 * @param pathname      fifo "identifier"
 * @param to_send       request to be sent
 * @return int          EXIT_FAILURE if error, EXIT_SUCCESS otherwise
 */
int client_send_request(char *pathname, message_t to_send);


int client_get_answer(char *pathname, int *fd, message_t *answer_received);

#endif //CLIENT_TBDNAME_H_INCLUDED