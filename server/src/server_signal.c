#include "server_signal.h"

#include <stdlib.h>
#include <signal.h>

static void dummy_handler(int n){}

int server_install_handlers(void){
    struct sigaction action;
    action.sa_handler = dummy_handler;
    if(sigemptyset(&action.sa_mask)) return EXIT_FAILURE;
    action.sa_flags = 0;
    if(sigaction(SIGALRM, &action, NULL)) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}