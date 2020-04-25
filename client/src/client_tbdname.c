#include "client_tbdname.h"

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>



long client_generate_random_ms(int lower, int upper) {

    srand(time(0));

    long random_ms = lower + (rand() % (upper - lower + 1));

    return random_ms;
}


int client_open_public_fifo(const char *pathname, int *fd) {

    if ( (*fd = open(pathname, O_RDONLY)) == -1) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}


int client_create_open_private_fifo(const char *pathname, mode_t mode, int *fd) {

    if (mkfifo(pathname, mode) != 0) return EXIT_FAILURE;

    if ( (*fd = open(pathname, O_RDONLY)) == -1) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int client_elim_private_fifo(const char *pathname, int *fd) {

    // close file descriptor
    if (close(*fd) == -1) return EXIT_FAILURE;

    // delete pathname from file system
    if (unlink(pathname) == -1) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}


message_t client_generate_rand_request(pthread_t tid, int *request_number) {

    message_t request_msg;

    int random_arg = client_generate_random_ms(100, 500);
    pid_t c_pid = getpid();

    request_msg.i = *request_number;
    request_msg.pid = c_pid;        // PID of client in this case
    request_msg.tid = tid;
    request_msg.dur = random_arg;
    request_msg.pl = REQUEST_PL_FIELD;

    return request_msg;
}

int client_create_thread(int *request_number) {

    pthread_t *tid = malloc(sizeof(pthread_t));
    t_args *thread_args_c = malloc(sizeof(t_args));

    thread_args_c->tid = tid;
    thread_args_c->request_number = request_number;

    pthread_create(tid, NULL, client_execute_thread, thread_args_c);

    // Updating request number
    ++*request_number;

    return EXIT_SUCCESS;
}

void *client_execute_thread(void *arg) {
    t_args *local_thread_args_c = (t_args *)arg;

    // create request 
    message_t to_send = client_generate_rand_request( *(local_thread_args_c->tid), local_thread_args_c->request_number);

    // send request via fifoname (arg)


    // get answer via (to be created) private channel

    return 0;
}