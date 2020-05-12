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

#define SECONDS_TO_MILLIS   1000    // from sec to ms

client_args_t args;                                                         // Arguments structure

int main(int argc, char *argv[]){
    // Initialize
    timeup_client = atomic_lli_ctor();                                      // Construct timeup_client
    if(client_args_ctor(&args, argc, argv)) return EXIT_FAILURE;            // Construct arguments structure from argc/argv
    if(client_threads_init())               return EXIT_FAILURE;            // Initialize threads
    // Time
    const double runtime_ms = SECONDS_TO_MILLIS * args.nsecs;               // Runtime of the program
    double start_time; if(common_gettime(&start_time)) return EXIT_FAILURE; // Start time
    double now_time;   if(common_gettime(&now_time))   return EXIT_FAILURE; // Time now
    // Launch threads
    for(int res = EXIT_SUCCESS, n = 0;
        !res && (now_time - start_time) < runtime_ms && !atomic_lli_get(timeup_client);
        res = common_gettime(&now_time), ++n){
        // Waiting between requests
        if(common_wait(random_range(10, 20))) return EXIT_FAILURE;
        // Create thread
        client_thread_args_t *thread_args = malloc(sizeof(client_thread_args_t));
        if(client_thread_args_ctor(thread_args, n, random_range(100, 200), args.fifoname)) return EXIT_FAILURE;
        pthread_t dummy;
        pthread_create(&dummy, NULL, client_execute_thread, thread_args);
    }

    client_wait_all_threads();
    // Cleanup
    atomic_lli_dtor(timeup_client);
    client_args_dtor(&args);
    if(client_threads_clear()) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}