#include "server_threads.h"

#include "common_time.h"
#include "common_atomic.h"
#include "output.h"

#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

#define SLEEP_MICROSECONDS 100000

atomic_lli_t *num_threads = NULL;

atomic_lli_t* num_processed_requests = NULL;

int server_threads_init(void){
    num_threads = atomic_lli_ctor();
    num_processed_requests = atomic_lli_ctor();

    return EXIT_SUCCESS;
}

int server_threads_clean(void){
    atomic_lli_dtor(num_threads); num_threads = NULL;
    atomic_lli_dtor(num_processed_requests); num_processed_requests = NULL;

    return EXIT_SUCCESS;
}

int server_thread_answer(const message_t *m, const message_t *confirm){
    int ret = EXIT_SUCCESS;
    char privfifo_path[PATH_MAX];
    if(sprintf(privfifo_path, "/tmp/%d.%lu", m->pid, m->tid) < 0) return EXIT_FAILURE;
    int privfifo_fd = open(privfifo_path, O_WRONLY);
    if(privfifo_fd < 0) return EXIT_FAILURE;
    if(write(privfifo_fd, confirm, sizeof(message_t)) != sizeof(message_t)) ret = EXIT_FAILURE;
    if(close(privfifo_fd)) ret = EXIT_FAILURE;
    return ret;
}

void* server_thread_func(void *arg){
    int *ret = malloc(sizeof(int));
    *ret = EXIT_SUCCESS;

    message_t *m = (message_t*)arg;

    /* Use bathroom */{
        /* Make confirmation message */
        message_t confirm; {
            confirm.i = m->i;
            confirm.pid = getpid();
            confirm.tid = pthread_self();
            confirm.dur = m->dur;
            confirm.pl = atomic_lli_postinc(num_processed_requests);
        }
        // Confirm usage of bathroom
        if(output(&confirm, op_ENTER)) { *ret = EXIT_FAILURE; return ret; }
        // Open, write and close private fifo
        if(server_thread_answer(m, &confirm)) { *ret = EXIT_FAILURE; return ret; }
        // Actually use bathroom
        if(common_wait(m->dur));
        // Finished using the bathroom
        if(output(&confirm, op_TIMUP)) { *ret = EXIT_FAILURE; return ret; }
    }
    //Routine stuff
    free(arg);
    atomic_lli_postdec(num_threads);
    return ret;
}

int server_create_thread(const message_t *m){
    atomic_lli_postinc(num_threads);

    message_t *m_ = malloc(sizeof(message_t));
    *m_ = *m;

    pthread_t tid_dummy;
    pthread_create(&tid_dummy, NULL, server_thread_func, m_);

    return EXIT_SUCCESS;
}

int server_wait_all_threads(void){
    while(true){
        if(atomic_lli_get(num_threads) <= 0) return EXIT_SUCCESS;
        if(usleep(SLEEP_MICROSECONDS)) return EXIT_FAILURE;
    }
}

int server_close_service(char* fifoname){
    int ret = EXIT_SUCCESS;
    
    // Open public fifo to clear "buffer"
    int fifo_des = open(fifoname, O_RDONLY);
    if(fifo_des == -1 && errno != EINTR) ret = EXIT_FAILURE;
    
    // Read one message (??)
    int r;
    message_t m;
    while((r = read(fifo_des, &m, sizeof(message_t))) == sizeof(message_t)){
        if(output(&m, op_2LATE)) ret = EXIT_FAILURE;

        message_t confirm; {
            confirm.i = m.i;
            confirm.pid = getpid();
            confirm.tid = pthread_self();
            confirm.dur = -1;
            confirm.pl = -1;
        }
        if (server_thread_answer(&m, &confirm)) ret = EXIT_FAILURE;
    }        

    close(fifo_des);
    if(unlink(fifoname)) ret = EXIT_FAILURE;

    if(server_wait_all_threads()) ret = EXIT_FAILURE;

    return ret;
}
