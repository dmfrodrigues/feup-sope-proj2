#include "server_signal.h"

#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>

static void sigalarm_handler(int n){
    (void)n;
    timeup_server = true;
}

int server_install_handlers(void){
    struct sigaction action;
    action.sa_handler = sigalarm_handler;
    if(sigemptyset(&action.sa_mask)) return EXIT_FAILURE;
    action.sa_flags = 0;
    if(sigaction(SIGALRM, &action, NULL)) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}