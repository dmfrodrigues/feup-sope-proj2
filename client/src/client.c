#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#include "client_args.h"
#include "client_tbdname.h"

/*
- [x] get args
- [x] function for random ms
- [-] function to open public fifo (fifoname arg)
- [-] create threads
- [ ] function to create request("pedido")
- [-] function to create/open private fifo
- [ ] send requests via public fifo
- [ ] check answer from server
- [ ] function to close/destroy private fifo
- [ ] close thread
- [ ] ...
*/

int main(int argc, char *argv[]){
    
    client_args_t *c_args = NULL;

    // get args
    client_args_ctor(c_args, argc, argv);

    // "fundamental operations"


    // "waiting interval" struct
    struct timespec *to_wait = NULL;

    int run_ms = 1000 * c_args->nsecs; // converting seconds to ms

    long start_time = clock(), elapsed;
    long end_time = start_time;

    // "throw" threads
    while ( (elapsed = (end_time - start_time) / CLOCKS_PER_SEC * 1000) < run_ms) {
        
        // waiting between requests
        to_wait->tv_nsec = NANOSECOND_MULTIPLIER * client_generate_random_ms(10, 20);
        if (nanosleep(to_wait, NULL) == -1) {
            // deal with error
            // EINTR -> interrupted by a signal
            // EINTR -> 1st arg is less than zero or greater/equal than 1000 million
        };

        // create thread

        // send request via fifoname (arg)

        // get answer via (to be created) private channel

        // close



        end_time = clock();
    }

    
    client_args_dtor(c_args);

    return 0;
}