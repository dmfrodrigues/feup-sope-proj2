#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>



int client_generate_random_ms(int lower, int upper) {

    srand(time(0));

    int random_ms = lower + (rand() % (upper - lower + 1));

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