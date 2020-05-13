#include "server_args.h"
#include "server_signal.h"
#include "message.h"
#include "output.h"
#include "server_threads.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>

#include "common_time.h"

#define MAX_THREADS 1000000
#define MAX_PLACES 1024

server_args_t args;
volatile sig_atomic_t timeup_server = false;

int main(int argc, char *argv[]){
    int ret = EXIT_SUCCESS;
    // Initialize
    if(server_args_ctor(&args, argc, argv, MAX_THREADS, MAX_PLACES)) return EXIT_FAILURE;        // Construct arguments structure from argc/argv
    if(server_threads_init(args.nplaces, args.nplaces)) return EXIT_FAILURE;        // Initialize threads
    if(server_install_handlers())                       return EXIT_FAILURE;        // Install alarm handler
    if(common_starttime(NULL)) return EXIT_FAILURE;
    if(alarm(args.nsecs)) return EXIT_FAILURE;
    mkfifo(args.fifoname, 0660);
    // if (sem_init(&s, NOT_SHARED, args.nthreads) != EXIT_SUCCESS) return EXIT_FAILURE;
    
    // Read requests
    message_t m;
    while(!timeup_server){ 

        // sem_wait(&s);

        // Open public fifo
        int fifo_des = open(args.fifoname, O_RDONLY);
        if(fifo_des == -1){
            if(errno == EINTR) break;
            else               return EXIT_FAILURE;
        }

        // Read messages
        int r;
        while((r = read(fifo_des, &m, sizeof(message_t))) == sizeof(message_t)){
            sem_wait(&s);
            if(output(&m, op_RECVD)){ ret = EXIT_FAILURE; break; }
            if(server_create_thread(&m)){ ret = EXIT_FAILURE; break; }
        }

        // Close public fifo
        close(fifo_des);
    }

    if (server_close_service(args.fifoname)) ret = EXIT_FAILURE;

    // Cleanup
    if(server_args_dtor(&args)) return EXIT_FAILURE;
    if(server_threads_clean())  return EXIT_FAILURE;

    // Destroy Semaphore
    if (sem_destroy(&s) != EXIT_SUCCESS) return EXIT_FAILURE;

    return ret;
}