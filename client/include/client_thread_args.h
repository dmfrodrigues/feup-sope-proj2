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
     * @brief Path of the fifo to where the request is to be sent
     */
    char *public_fifoname;
} client_thread_args_t;

/**
 * @brief client_thread constructor.
 * 
 * @param p Pointer to object to construct
 * @param i Number of the request
 * @param dur Duration of occupancy
 * @param public_fifoname Path of the fifo to where the request is to be sent
 * @return int EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int client_thread_args_ctor(client_thread_args_t *p, int i, int dur, const char *public_fifoname);

/**
 * @brief client_thread destructor.
 * 
 * Destructs internal structures, but does not free the pointer; you have to free it yourself.
 * 
 * @param p Pointer to object to destruct
 * @return int EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int client_thread_args_dtor(client_thread_args_t *p) __attribute__((warn_unused_result));

#endif //CLIENT_THREAD_ARGS_H_INCLUDED
