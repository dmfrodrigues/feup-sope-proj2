#include "client_tbdname.h"
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>

#include <pthread.h>



long client_generate_random_ms(int lower, int upper) {

    srand(time(0));

    long random_ms = lower + (rand() % (upper - lower + 1));

    return random_ms;
}


int client_open_public_fifo(const char *pathname, mode_t mode, int *fd) {

    if ( (*fd = open(pathname, O_RDONLY)) == -1) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}


int client_create_open_private_fifo(const char *pathname, mode_t mode, int *fd) {

    if (mkfifo(pathname, mode) != 0) return EXIT_FAILURE;

    if ( (*fd = open(pathname, O_RDONLY)) == -1) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}


int client_generate_rand_param() {

    int random_arg = client_generate_random_ms(1, 100);


}

int client_create_thread() {

    pthread_t tid;
    pthread_create(&tid, NULL, client_execute_thread, NULL);

}

void *client_execute_thread(void *arg) {

}