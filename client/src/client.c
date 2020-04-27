#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>


#include "client_args.h"
#include "client_thread.h"
#include "common_time.h"
#include "utils.h"

#define NANOSECOND_MULT     1000000 // from ms to ns
#define MILLISECOND_MULT    1000    // from sec to ms

client_args_t c_args;

int client_init(int argc, char *argv[]){
    client_args_ctor(&c_args, argc, argv);
    return EXIT_SUCCESS;
}

void client_clean(void){
    client_args_dtor(&c_args);
}

int main(int argc, char *argv[]){
    // Initialize
    if(client_init(argc, argv)) return EXIT_FAILURE;
    if(atexit(client_clean)) return EXIT_FAILURE;
    // Time
    const double run_ms = MILLISECOND_MULT * c_args.nsecs;      // Time to run program
    if(common_starttime(NULL)) return EXIT_FAILURE;             // Start timer
    double time = 0; if(common_gettime(&time)) return EXIT_FAILURE; // Get initial value for timer
    // Launch threads
    int res = 0;
    for(int n = 0; time < run_ms; (res = common_gettime(&time)), ++n){
        if(res) return EXIT_FAILURE;
        // Waiting between requests
        struct timespec to_wait = {
            .tv_sec = 0,
            .tv_nsec = NANOSECOND_MULT*random_range(10, 20)
        };
        if (nanosleep(&to_wait, NULL)) return EXIT_FAILURE;
        // Create thread
        client_thread_args_t *args = malloc(sizeof(client_thread_args_t));
        if(client_thread_args_ctor(args, n, random_range(100, 200), c_args.fifoname)) return EXIT_FAILURE;
        pthread_t dummy;
        pthread_create(&dummy, NULL, client_execute_thread, args);
    }

    client_wait_all_threads();

    return EXIT_SUCCESS;
}