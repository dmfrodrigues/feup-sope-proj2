#ifndef CLIENT_THREAD_ARGS_H_INCLUDED
#define CLIENT_THREAD_ARGS_H_INCLUDED

/**
 * @brief struct used to pass multiple arguments to the thread function
 * 
 */
typedef struct {
    /**
     * @brief Number of the request to be sent
     */
    int i;
    /**
     * @brief Duration of occupancy
     */
    int dur;
    /**
     * @brief pathname of the fifoname to where the request is to be sent
     */
    char *public_fifoname;
} client_thread_args_t;

int client_thread_args_ctor(client_thread_args_t *p, int i, int dur, const char *public_fifoname);

int client_thread_args_dtor(client_thread_args_t *p);

#endif //CLIENT_THREAD_ARGS_H_INCLUDED
