#include "client_threads.h"
#include "client_thread_args.h"
#include "output.h"
#include "common_atomic.h"
#include "client_time.h"


#include <pthread.h>
#include <linux/limits.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <stdbool.h>

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
    if (fd_public_fifo == -1) {
        if (errno == ENOENT) return 2;
        else return EXIT_FAILURE;
    }
    if (write(fd_public_fifo, &to_send, sizeof(message_t)) == -1)   return EXIT_FAILURE;    // Write message to fifo
    if (close(fd_public_fifo))                                      return EXIT_FAILURE;    // Close fifo
    if (output(&to_send, op_IWANT))                                 return EXIT_FAILURE;    // Log
    return EXIT_SUCCESS;
}

static atomic_lli_t *num_threads = NULL;                // Number of active threads
int client_threads_init(void){
    num_threads = atomic_lli_ctor();                    // Initialize num_threads
    if(num_threads == NULL) return EXIT_FAILURE;
    return EXIT_SUCCESS;
}
int client_threads_clear(void){
    atomic_lli_dtor(num_threads); num_threads = NULL;   // Destruct num_threads
    return EXIT_SUCCESS;
}

void *client_execute_thread(void *arg) {
    int *ret = malloc(sizeof(int)); *ret = EXIT_SUCCESS;                // Allocate for ret

    if(atomic_lli_inc(num_threads)) {*ret = EXIT_FAILURE; return ret; } // Increment num_threads (as we are entering a new thread)

    client_thread_args_t *args = (client_thread_args_t *)arg;           // Convert to proper argument type
    // Create message
    message_t m;
    m.i   = args->i;
    m.pid = getpid();
    m.tid = pthread_self();
    m.dur = args->dur;
    m.pl  = REQUEST_PL_FIELD;
    // Create private fifo
    char private_fifo_path[PATH_MAX];
    if(sprintf(private_fifo_path, "/tmp/%d.%lu", getpid(), pthread_self()) < 0) {*ret = EXIT_FAILURE; return ret; }
    if(mkfifo(private_fifo_path, 0660))                                         {*ret = EXIT_FAILURE; return ret; }
    // Send request via fifoname
    int req;
    if ((req = client_send_request(args->public_fifoname, m)) != EXIT_SUCCESS){ 
        if (req == 2){
            m.dur = -1;
            m.pl = -1;
            if(output(&m, op_FAILD))            {*ret = EXIT_FAILURE; return ret; }             // Output FAILD message
            if(unlink(private_fifo_path))       {*ret = EXIT_FAILURE; return ret; }             // Delete fifo
            if(client_thread_args_dtor(args))   {*ret = EXIT_FAILURE; return ret; } free(args); // Free arguments
            if(atomic_lli_dec(num_threads))     {*ret = EXIT_FAILURE; return ret; }             // Decrement number of threads
            *ret = EXIT_SUCCESS; return ret; 
        }
        *ret = EXIT_FAILURE; return ret;
    }
    // Open private fifo
    int private_fifo_filedes = open(private_fifo_path, O_RDONLY); 
    if (private_fifo_filedes == -1){ *ret = EXIT_FAILURE; return ret; }
    // Receive answer
    message_t ans;
    if(read(private_fifo_filedes, &ans, sizeof(message_t)) != sizeof(message_t)){ *ret = EXIT_FAILURE; return ret; }
    if(ans.pl != -1) {
        if(output(&ans, op_IAMIN)){ *ret = EXIT_FAILURE; return ret; }
    } else {
        if(output(&ans, op_CLOSD)){ *ret = EXIT_FAILURE; return ret; }
        atomic_lli_set(timeup_client, 1);
    }
    if(close(private_fifo_filedes))     { *ret = EXIT_FAILURE; return ret; }                // Close private fifo
    if(unlink(private_fifo_path))       { *ret = EXIT_FAILURE; return ret; }                // Delete fifo
    if(client_thread_args_dtor(args))   { *ret = EXIT_FAILURE; return ret; } free(args);    // Free arguments
    if(atomic_lli_dec(num_threads))     { *ret = EXIT_FAILURE; return ret; }                // Decrement num_threads (as we are about to exit this thread)
    return ret;
}

#define SLEEP_MICROSECONDS 10000

int client_wait_all_threads(void){
    long long int num_threads_value;
    while(true){
        if(atomic_lli_get(num_threads, &num_threads_value)) return EXIT_FAILURE;
        if(num_threads_value <= 0) return EXIT_SUCCESS;
        if(usleep(SLEEP_MICROSECONDS)) return EXIT_FAILURE;
    }
}
