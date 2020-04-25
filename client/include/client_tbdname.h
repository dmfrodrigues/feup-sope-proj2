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

} t_args;


/**
 * @brief Generates a random number of ms
 * 
 * @param lower 
 * @param upper 
 * @return long 
 */
long client_generate_random_ms(int lower, int upper); // use default values: lower = 0, upper = 10

/**
 * @brief Opens public fifo previously created by the server
 * 
 * @param pathname      fifo pathname
 * @param fd            file descriptor
 * @return int          EXIT_FAILURE if error, EXIT_SUCCESS otherwise
 */
int client_open_public_fifo(const char *pathname, int *fd);



int client_create_open_private_fifo(const char *pathname, mode_t mode, int *fd); // use default values: mode = 0660

/**
 * @brief Closes file descriptor and destroys fifo (unlink)
 * 
 * @param pathname 
 * @param fd 
 * @return int 
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
 * @return int 
 */
int client_create_thread(int *request_number);


void *client_execute_thread(void *arg);

#endif //CLIENT_TBDNAME_H_INCLUDED