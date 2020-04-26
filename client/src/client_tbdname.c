#include "client_tbdname.h"

#include <pthread.h>
#include <linux/limits.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>


long client_generate_random_ms(int lower, int upper) {

    srand(time(0));

    long random_ms = lower + (rand() % (upper - lower + 1));

    return random_ms;
}


int client_open_public_fifo(const char *pathname, int *fd) {

    if ( (*fd = open(pathname, O_WRONLY)) == -1) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int client_create_private_fifo(mode_t mode, pid_t pid, pthread_t tid, char *pathname) {

    // Create pathname for fifo
    sprintf(pathname, "/tmp/%d.%ld", pid, tid);
    
    // Create fifo itself
    if (mkfifo(pathname, mode) != 0) return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int client_open_private_fifo(const char *pathname, int *fd) {

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

int client_create_thread(int *request_number, char *fifoname) {

    pthread_t *tid = malloc(sizeof(pthread_t));
    t_args *thread_args_c = malloc(sizeof(t_args));

    thread_args_c->tid = tid;
    thread_args_c->request_number = request_number;
    thread_args_c->public_fifoname = fifoname;

    pthread_create(tid, NULL, client_execute_thread, thread_args_c);

    // Updating request number
    ++*request_number;

    return EXIT_SUCCESS;
}

void *client_execute_thread(void *arg) {
    t_args *local_thread_args_c = (t_args *)arg;

    // Create request 
    message_t to_send = client_generate_rand_request( *(local_thread_args_c->tid), local_thread_args_c->request_number);

    int local_get_pid = getpid();
    char pathname[PIPE_BUF] = "";

    // Create (private) fifo (to receive answer from server)
    client_create_private_fifo(0660, local_get_pid, *(local_thread_args_c->tid), pathname);

    // Send request via fifoname (argument) - public fifo
    if (client_send_request(local_thread_args_c->public_fifoname, to_send) != EXIT_SUCCESS) return (void *) EXIT_FAILURE;

    int *fd = 0; // is there a correct way not to have the unitialized warning?
    // Open (private) fifo
    if (client_open_private_fifo(pathname, fd) != EXIT_SUCCESS) return (void *) EXIT_FAILURE;

    message_t answer_received;
    // Get answer from server
    if (client_get_answer(pathname, fd, &answer_received) != EXIT_SUCCESS) return (void *) EXIT_FAILURE;
    
    // Close/Eliminate (private) fifo
    if (client_elim_private_fifo(pathname, fd) != EXIT_SUCCESS) return (void *) EXIT_FAILURE;

    free(local_thread_args_c->tid);
    free(local_thread_args_c->request_number);
    free(local_thread_args_c->public_fifoname);
    free(local_thread_args_c);

    return (void *) EXIT_SUCCESS;
}


int client_send_request(char *pathname, message_t to_send) {

    int fd_public_fifo;

    // Open fifo for writing
    if (client_open_public_fifo(pathname, &fd_public_fifo) != 0) {
        return EXIT_FAILURE;
    }

    // Write message to fifo
    if (write(fd_public_fifo, &to_send, sizeof(message_t)) == -1) {
        return EXIT_FAILURE;
    }

    close(fd_public_fifo);

    return EXIT_SUCCESS;
}

int client_get_answer(char *pathname, int *fd, message_t *answer_received) {

    if (read(*fd, answer_received, PIPE_BUF) == -1) return EXIT_FAILURE;

    // answer_received = (message_t *) buf;

    return EXIT_SUCCESS;
}