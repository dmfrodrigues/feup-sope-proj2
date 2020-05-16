#ifndef CLIENT_ARGS_H_INCLUDED
#define CLIENT_ARGS_H_INCLUDED

/**
 * @brief Holds all argument options with which the client was called.
 */
typedef struct {
    /**
     * -t
     * number of seconds the program should be running
     */
    int nsecs;
    /**
     * @brief Named pipe the client must connect to.
     */
    char *fifoname;
} client_args_t;

/**
 * @brief client_args constructor.
 * 
 * @param p     Pointer to client_args_t to construct
 * @param argc  Size of argv
 * @param argv  Arguments passed to program, to fill client_args_t
 * @return int  EXIT_SUCCESS if successful, EXIT_FAILURE otherwise
 */
int client_args_ctor(client_args_t *p, int argc, char *argv[]);

/**
 * @brief client_args destructor.
 * 
 * @param p     Pointer to client_args_t to destruct
 * @return int  EXIT_SUCCESS if successful
 */
int client_args_dtor(client_args_t *p) __attribute__((warn_unused_result));

#endif //CLIENT_ARGS_H_INCLUDED
