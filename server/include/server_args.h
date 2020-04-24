#ifndef SERVER_ARGS_H_INCLUDED
#define SERVER_ARGS_H_INCLUDED

/**
 * @brief Holds all argument options with which the server was called.
 */
typedef struct {
    /**
     * -t
     * number of seconds the program should be running
     */
    int nsecs;
    /**
     * -l
     * maximum occupation of the bathroom
     */
    int nplaces;
    /**
     * -n
     * (maximum) number of threads handling requests
     */
    int nthreads;
    /**
     * @brief Named pipe the server must create.
     */
    char *fifoname;
} server_args_t;

/**
 * @brief server_args constructor.
 * 
 * @param p     Pointer to server_args_t to construct
 * @param argc  Size of argv
 * @param argv  Arguments passed to program, to fill server_args_t
 * @return int  EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int server_args_ctor(server_args_t *p, int argc, char *argv[], int max_threads);

/**
 * @brief server_args destructor.
 * 
 * @param p     Pointer to server_args_t to destruct
 * @return int  EXIT_SUCCESS if successful
 */
int server_args_dtor(server_args_t *p);

#endif //SERVER_ARGS_H_INCLUDED
