#include "server_args.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <limits.h>

static const server_args_t SERVER_ARGS_DEFAULT = { 
    .nsecs = -1,
    .nplaces = INT_MAX,
    .nthreads = 1000000000,
    .fifoname = NULL
};

static const char OPTSTRING[] = "t:l:n:";

static const char FIFO_PREFIX[] = "/tmp/";

int server_args_ctor(server_args_t *p, int argc, char *argv[], int max_threads, int max_places){
    // Initial work
    if(p == NULL || argv == NULL){ errno = EINVAL; return EXIT_FAILURE; }                       // Invalid arguments
    *p = SERVER_ARGS_DEFAULT;                                                                   // Set return value to default
    p->nthreads = max_threads;                                                                  // Set max number of threads
    p->nplaces = max_places;
    // Cycle through arguments
    optind = 1;
    int opt;
    while((opt = getopt(argc, argv, OPTSTRING)) != -1){                                         // While there are options to process
        switch(opt){
            case 't': if(sscanf(optarg, "%d", &p->nsecs   ) != 1) return EXIT_FAILURE; break;   // Time
            case 'l': if(sscanf(optarg, "%d", &p->nplaces ) != 1) return EXIT_FAILURE; break;   // Number of places
            case 'n': if(sscanf(optarg, "%d", &p->nthreads) != 1) return EXIT_FAILURE; break;   // Number of threads
            case '?':                                             return EXIT_FAILURE;          // Unknown characters
            default: optind = 1;                                  return EXIT_FAILURE;          // Other errors
        }
    }

    if (p->nsecs    == SERVER_ARGS_DEFAULT.nsecs    ||                                          // If time was not set, or
        p->nthreads > max_threads ||                                                            // if the number of threads is larger than the max number of threads, or
        argc - optind != 1){                                                                    // if there are more than one arguments that are not options
        errno = EINVAL;
        return EXIT_FAILURE;
    }
    // Get fifoname
    p->fifoname = calloc(strlen(argv[optind])+strlen(FIFO_PREFIX)+1, sizeof(char));
    if(p->fifoname == NULL) return EXIT_FAILURE;
    strcat(strcpy(p->fifoname, FIFO_PREFIX), argv[optind]);

    return EXIT_SUCCESS;
}

int server_args_dtor(server_args_t *p){
    if(p == NULL) return EXIT_SUCCESS;
    free(p->fifoname);
    p->fifoname = NULL;
    return EXIT_SUCCESS;
}
