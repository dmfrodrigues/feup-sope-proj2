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

client_args_t args;                                                                     // Arguments structure

int main(int argc, char *argv[]){
    // Initialize
    if((timeup_client = atomic_lli_ctor()) == NULL) return EXIT_FAILURE;                // Construct timeup_client
    if(client_args_ctor(&args, argc, argv))         return EXIT_FAILURE;                // Construct arguments structure from argc/argv
    if(client_threads_init())                       return EXIT_FAILURE;                // Initialize threads
    // Time
    const double runtime_ms = SECONDS_TO_MILLIS * args.nsecs;                           // Runtime of the program
    double start_time; if(common_gettime(&start_time)) return EXIT_FAILURE;             // Start time
    double now_time;   if(common_gettime(&now_time))   return EXIT_FAILURE;             // Time now
    // Launch threads
    for(int res = EXIT_SUCCESS, n = 0;                                                  // res is used to check for errors in common_gettime; n is the request number
        !res && (now_time - start_time) < runtime_ms && !atomic_lli_get(timeup_client); // While there is no error, time has not run out and we were not notified of the bathroom closing
        res = common_gettime(&now_time), ++n){                                          // Get new value for now_time, increment n
        if(common_wait(random_range(10, 20))) return EXIT_FAILURE;                      // Waiting between requests
        // Create thread
        client_thread_args_t *thread_args = malloc(sizeof(client_thread_args_t));
        if(thread_args == NULL)                                                             return EXIT_FAILURE;
        if(client_thread_args_ctor(thread_args, n, random_range(100, 200), args.fifoname))  return EXIT_FAILURE;
        pthread_t dummy; pthread_create(&dummy, NULL, client_execute_thread, thread_args);
    }
    client_wait_all_threads();                                                          // Wait for all threads to finish
    // Cleanup
    if(atomic_lli_dtor(timeup_client))  return EXIT_FAILURE;                            // Destruct timeup_client
    if(client_args_dtor(&args))         return EXIT_FAILURE;                            // Destruct arguments structure
    if(client_threads_clear())          return EXIT_FAILURE;                            // Clean up threads

    return EXIT_SUCCESS;
}