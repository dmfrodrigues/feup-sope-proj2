#include "server_args.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <limits.h>

static const server_args_t server_args_default = { 
    .nsecs = -1,
    .nplaces = INT_MAX,
    .nthreads = 1000000000,
    .fifoname = NULL
};

static const char OPTSTRING[] = "t:l:n:";

static const char FIFO_PREFIX[] = "/tmp/";

int server_args_ctor(server_args_t *p, int argc, char *argv[], int max_threads, int max_places){
    *p = server_args_default;
    p->nthreads = max_threads;
    p->nplaces = max_places;
    char buf[1024];

    if(p == NULL || argv == NULL){ errno = EINVAL; return EXIT_FAILURE; }

    opterr = 0;
    optind = 1;
    int opt = 0;
    while((opt = getopt(argc, argv, OPTSTRING)) != -1){
        switch(opt){
            case 't': if(sscanf(optarg, "%d", &p->nsecs   ) != 1) return EXIT_FAILURE; break;
            case 'l': if(sscanf(optarg, "%d", &p->nplaces ) != 1) return EXIT_FAILURE; break;
            case 'n': if(sscanf(optarg, "%d", &p->nthreads) != 1) return EXIT_FAILURE; break;
            case '?':
                sprintf(buf, "server: invalid option -- '%c'\n", optopt);
                write(STDERR_FILENO, buf, strlen(buf));
                return EXIT_FAILURE;
            default:
                opterr = 1; optind = 1; return EXIT_FAILURE;
        }
    }
    opterr = 1;

    if (p->nsecs    == server_args_default.nsecs    ||
        p->nthreads > max_threads ||
        argc - optind != 1){
        errno = EINVAL;
        return EXIT_FAILURE;
    }

    p->fifoname = calloc(strlen(argv[optind])+strlen(FIFO_PREFIX)+1, sizeof(char));
    strcat(strcpy(p->fifoname, FIFO_PREFIX), argv[optind]);

    return EXIT_SUCCESS;
}

int server_args_dtor(server_args_t *p){
    if(p == NULL) return EXIT_SUCCESS;
    free(p->fifoname);
    p->fifoname = NULL;
    return EXIT_SUCCESS;
}
