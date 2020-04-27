#include "client_thread_args.h"

#include <stdlib.h>
#include <string.h>

int client_thread_args_ctor(client_thread_args_t *p, int i, int dur, const char *public_fifoname){
    if(p == NULL) return EXIT_FAILURE;
    p->i = i;
    p->dur = dur;
    p->public_fifoname = calloc(strlen(public_fifoname)+1, sizeof(char));
    strcpy(p->public_fifoname, public_fifoname);
    return EXIT_SUCCESS;
}

int client_thread_args_dtor(client_thread_args_t *p){
    free(p->public_fifoname);
    p->public_fifoname = NULL;
    return EXIT_SUCCESS;
}
