#include "client_args.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>

static const client_args_t CLIENT_ARGS_DEFAULT = { 
    .nsecs = -1,
    .fifoname = NULL
};

static const char OPTSTRING[] = "t:";

static const char FIFO_PREFIX[] = "/tmp/";

int client_args_ctor(client_args_t *p, int argc, char *argv[]){
    // Initial work
    if(p == NULL || argv == NULL){ errno = EINVAL; return EXIT_FAILURE; }                   // Invalid arguments
    *p = CLIENT_ARGS_DEFAULT;                                                               // Set return value to default
    // Cycle through arguments
    optind = 1;
    int opt;
    while((opt = getopt(argc, argv, OPTSTRING)) != -1){                                     // While there are options to process
        switch(opt){
            case 't': if(sscanf(optarg, "%d", &p->nsecs) != 1)  return EXIT_FAILURE; break; // Time
            case '?':                                           return EXIT_FAILURE;        // Unknown characters
            default: optind = 1;                                return EXIT_FAILURE;        // Other errors
        }
    }

    if (p->nsecs == CLIENT_ARGS_DEFAULT.nsecs ||                                            // If time was not set, or
        argc - optind != 1) return EXIT_FAILURE;                                            // if there are more than one arguments that are not options
    // Get fifoname
    p->fifoname = calloc(strlen(argv[optind])+strlen(FIFO_PREFIX)+1, sizeof(char));
    strcat(strcpy(p->fifoname, FIFO_PREFIX), argv[optind]);

    return EXIT_SUCCESS;
}

int client_args_dtor(client_args_t *p){
    if(p == NULL) return EXIT_SUCCESS;
    free(p->fifoname);
    p->fifoname = NULL;
    return EXIT_SUCCESS;
}
