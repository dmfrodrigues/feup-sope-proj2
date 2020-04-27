#include "client_thread.h"
#include "client_thread_args.h"
#include "output.h"

#include <pthread.h>
#include <linux/limits.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>

#define REQUEST_PL_FIELD    -1      // message_t pl field for request

/**
 * @brief Sends request via the public fifo to the server
 * 
 * @param pathname      (public) fifo "identifier"
 * @param to_send       request to be sent
 * @return int          EXIT_FAILURE if error, EXIT_SUCCESS otherwise
 */
int client_send_request(char *pathname, message_t to_send) {
    // Open fifo for writing
    int fd_public_fifo = open(pathname, O_WRONLY);
    if (fd_public_fifo == -1) return EXIT_FAILURE;
    // Write message to fifo
    if (write(fd_public_fifo, &to_send, sizeof(message_t)) == -1) return EXIT_FAILURE;
    // Close fifo
    close(fd_public_fifo);
    // Log
    if(output(&to_send, op_IWANT)) return EXIT_FAILURE;
    // Return
    return EXIT_SUCCESS;
}

void *client_execute_thread(void *arg) {
    int *ret = malloc(sizeof(int)); *ret = EXIT_SUCCESS;
    client_thread_args_t *args = (client_thread_args_t *)arg;
    // Create message 
    message_t m;
    m.i   = args->i;
    m.pid = getpid();
    m.tid = pthread_self();
    m.dur = args->dur;
    m.pl  = REQUEST_PL_FIELD;
    // Create private fifo
    char privfifo_path[PATH_MAX];
    sprintf(privfifo_path, "/tmp/%d.%lu", getpid(), pthread_self());
    mkfifo(privfifo_path, 0660);
    // Send request via fifoname
    if (client_send_request(args->public_fifoname, m) != EXIT_SUCCESS){ *ret = EXIT_FAILURE; return ret; }

    // Open private fifo
    int privfifo_fd = open(privfifo_path, O_RDONLY);
    if (privfifo_fd == -1){ *ret = EXIT_FAILURE; return ret; }
    // Receive answer
    message_t ans;
    if(read(privfifo_fd, &ans, sizeof(message_t)) != sizeof(message_t)){ *ret = EXIT_FAILURE; return ret; }
    if(ans.pl != -1) {
        if(output(&ans, op_IAMIN));
    } else {
        if(output(&ans, op_CLOSD));
    }
    // Close private fifo
    close(privfifo_fd);

    // Delete fifo
    unlink(privfifo_path);
    // Free arguments
    if(client_thread_args_dtor(args)){ *ret = EXIT_FAILURE; return ret; }
    free(args);
    // Return
    return ret;
}
