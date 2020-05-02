#include "client_args.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>

static const client_args_t client_args_default = 
{ 
.nsecs = -1,
.fifoname = NULL
};

static const char optstring[] = "t:";

static const char fifo_prefix[] = "/tmp/";

int client_args_ctor(client_args_t *p, int argc, char *argv[]){
    *p = client_args_default;
    char buf[1024];

    if(p == NULL || argv == NULL){ errno = EINVAL; return EXIT_FAILURE; }

    opterr = 0;
    optind = 1;
    int opt = 0;
    while((opt = getopt(argc, argv, optstring)) != -1){
        switch(opt){
            case 't': if(sscanf(optarg, "%d", &p->nsecs) != 1) return EXIT_FAILURE; break;
            case '?':
                sprintf(buf, "client: invalid option -- '%c'\n", optopt);
                write(STDERR_FILENO, buf, strlen(buf));
                return EXIT_FAILURE;
            default:
                opterr = 1; optind = 1; return EXIT_FAILURE;
        }
    }
    opterr = 1;

    if (p->nsecs == client_args_default.nsecs ||
        argc - optind != 1){
        errno = EINVAL;
        return EXIT_FAILURE;
    }

    p->fifoname = calloc(strlen(argv[optind])+strlen(fifo_prefix)+1, sizeof(char));
    strcat(strcpy(p->fifoname, fifo_prefix), argv[optind]);

    return EXIT_SUCCESS;
}

int client_args_dtor(client_args_t *p){
    if(p == NULL) return EXIT_SUCCESS;
    free(p->fifoname);
    p->fifoname = NULL;
    return EXIT_SUCCESS;
}
