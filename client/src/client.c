#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#include "client_args.h"
#include "client_tbdname.h"

/*
- get args
- function for random ms
- function to open public fifo (fifoname arg)
- create threads
- function to create request("pedido")
- function to create/open private fifo
- send requests via public fifo
- check answer from server
- function to close/destroy private fifo
- close thread
- ...
*/

int main(int argc, char *argv[]){
    
    int n = 0;
    int *request_number = &n;         // start at 0 or 1 ?
    *request_number = 0;

    client_args_t *c_args = NULL;

    // get args
    client_args_ctor(c_args, argc, argv);

    // "fundamental operations"
    

    // "waiting interval" struct
    struct timespec *to_wait = NULL;

    int run_ms = MILLISECOND_MULT * c_args->nsecs; // converting seconds to ms

    long start_time = clock(), elapsed;
    long end_time = start_time;

    // "throw" threads
    while ( (elapsed = (end_time - start_time) / CLOCKS_PER_SEC * MILLISECOND_MULT) < run_ms) {
        
        // waiting between requests
        to_wait->tv_nsec = NANOSECOND_MULT * client_generate_random_ms(10, 20);
        if (nanosleep(to_wait, NULL) == -1) {
            // deal with error
            // EINTR -> interrupted by a signal
            // EINTR -> 1st arg is less than zero or greater/equal than 1000 million
        };

        // create thread
        if (client_create_thread(request_number, c_args->fifoname) != EXIT_SUCCESS) {
            // handle error
        }

        end_time = clock();
    }

    
    client_args_dtor(c_args);
    free(request_number);

    return 0;
}