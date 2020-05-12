#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#include "client_args.h"
#include "client_threads.h"
#include "common_time.h"
#include "common_atomic.h"
#include "client_time.h"
#include "utils.h"

#define NANOSECOND_MULT     1000000 // from ms to ns
#define MILLISECOND_MULT    1000    // from sec to ms

client_args_t c_args;
atomic_lli_t *timeup_client = NULL;

int main(int argc, char *argv[]){
    // Initialize
    timeup_client = atomic_lli_ctor();
    if(client_args_ctor(&c_args, argc, argv)) return EXIT_FAILURE;
    if(client_threads_init()) return EXIT_FAILURE;
    // Time
    const double run_ms = MILLISECOND_MULT * c_args.nsecs;      // Time to run program
    if(common_starttime(NULL)) return EXIT_FAILURE;             // Start timer
    double time = 0; if(common_gettime(&time)) return EXIT_FAILURE; // Get initial value for timer
    // Launch threads
    int res = 0;
    for(int n = 0; time < run_ms && !atomic_lli_get(timeup_client); (res = common_gettime(&time)), ++n){
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
    // Cleanup
    atomic_lli_dtor(timeup_client);
    client_args_dtor(&c_args);
    if(client_threads_clear()) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}